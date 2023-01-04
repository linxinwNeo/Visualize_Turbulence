#include "ECG.h"
#include "Geometry/Mesh.h"
#include "Geometry/Tet.h"
#include <set>

ECG_EDGE::ECG_EDGE()
{
    this->nodes[0] = this->nodes[1] = NULL;
    this->sl = NULL;
}


// don't delete anything
// ECG will take care of deletion
ECG_EDGE::~ECG_EDGE()
{
    this->nodes[0] = this->nodes[1] = NULL;
    this->sl = NULL;
}

void ECG_EDGE::set(ECG_NODE* first, ECG_NODE* sec)
{
    this->nodes[0] = first;
    this->nodes[1] = sec;
}

void ECG_EDGE::set(ECG_NODE *first, ECG_NODE *sec, StreamLine *sl)
{
    this->nodes[0] = first;
    this->nodes[1] = sec;
    this->sl = sl;
}


ECG::ECG(const double t)
{
    this->t = t;
}

ECG::~ECG()
{
    for(Singularity* sing : this->sings){
        if(sing!=NULL) delete sing;
    }
    this->sings.clear();


    for(ECG_NODE* node : this->nodes){
        if(node != NULL) delete node;
    }
    this->nodes.clear();


    for(ECG_EDGE* e : this->edges){
        if(e!=NULL) delete e;
    }

    this->edges.clear();


    for(StreamLine* sl : this->sls){
        if(sl != NULL) delete sl;
    }

    this->sls.clear();
}

void ECG::add_sing(Singularity * sing)
{
    this->sings.push_back(sing);
}

void ECG::add_node(ECG_NODE * node)
{
    this->nodes.push_back(node);
}

void ECG::add_edge(ECG_EDGE * ecg_e)
{
    this->edges.push_back(ecg_e);
}

void ECG::add_sl(StreamLine * sl)
{
    this->sls.push_back(sl);
}


// call this function when sings are not empty.
// for each singularity, randomly placing unique num_of_seeds seeds around it
// construct inital streamlines using those seeds and return.
#define TRIPLE pair<double, pair<double, double>>
vector<vector<StreamLine*>> ECG::placing_random_seeds(Mesh* mesh, UL num_of_seeds) const
{
    vector<vector<StreamLine*>> sls_for_all_sings;
    // data check
    if(num_of_seeds == 0) {
        qDebug() << "ECG::placing_random_seeds: num_of_seeds is 0!";
        return sls_for_all_sings;
    }

    if(this->num_sings() == 0) {
        qDebug() << "ECG::placing_random_seeds: empty singularity list!";
        return sls_for_all_sings;
    }

    const double dist = 1e-5; // all randomly points are within dist to the singularity
    const double divider = 1./dist;
    srand((unsigned)time(NULL));

    // for each singularity
    for(Singularity* sing : this->sings){
        vector<StreamLine*> sls;
        // randomly placing unique num_of_seeds seeds around it
        set<TRIPLE> used; // fake triple in c++
        UL cur_num_of_seeds = 0;
        while(cur_num_of_seeds < num_of_seeds){
            // generate a randomly numebr between -dist and dist
            const double dx=((double)rand() / (double)RAND_MAX) / divider * 2 - dist;
            const double dy=((double)rand() / (double)RAND_MAX) / divider * 2 - dist;
            const double dz=((double)rand() / (double)RAND_MAX) / divider * 2 - dist;
            TRIPLE new_triple = {dx, {dy, dz}};
            if( used.find(new_triple) == used.end() ){ // not in set
                used.insert(new_triple);
                Vector3d new_cords = sing->cords;
                new_cords.set( new_cords.x() + dx,  new_cords.y() + dy, new_cords.z() + dz );
                // find the Tet that contains this coordinate
                Tet* sing_tet = sing->in_which_tet;
                double ds[4];
                Tet* new_cords_tet = mesh->inWhichTet(new_cords, sing_tet, ds); // find the corresponding tet
                if(new_cords_tet == NULL) continue;
                // interpolate this coordinate and obtain a vertex
                Vertex* new_vert = new_cords_tet->get_vert_at(new_cords, t, ds, false, true);
                // build the new streamline
                StreamLine* sl = new StreamLine(new_vert, t);
                sls.push_back(sl);
                // increment and go to make another seed
                cur_num_of_seeds ++;
            }
        }
        sls_for_all_sings.push_back(sls);
    }

    return sls_for_all_sings;
}


// call this function after sings are filled.
// build ECG_NODES using sings
// one singularity is one ECG_NODE
void ECG::build_ECG_NODES()
{

}


// given the streamline seeds, we trace them,
// at any time, if we see a streamline is really close to another singulairty while tracing, we stop stracing and
// constrcuct an directed edge between two ECG nodes.
void ECG::build_ECG_EDGES(Mesh *mesh, vector<vector<StreamLine *> > sls_for_all_sings)
{

}



ECG_NODE::ECG_NODE(Singularity *sing)
{
    this->node = sing;
}


void ECG_NODE::add_nodes(ECG_NODE *n)
{
    this->nodes.push_back(n);
}


void ECG_NODE::add_edges(ECG_EDGE* e)
{
    this->edges.push_back(e);
}
