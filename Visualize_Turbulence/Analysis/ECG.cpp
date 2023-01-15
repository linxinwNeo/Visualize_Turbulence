#include "ECG.h"
#include "Geometry/Mesh.h"
#include "Geometry/Tet.h"
#include "Others/Utilities.h"
#include <set>

ECG_EDGE::ECG_EDGE()
{
    this->nodes[0] = this->nodes[1] = nullptr;
    this->sl = nullptr;
}

ECG_EDGE::ECG_EDGE(ECG_NODE *first, ECG_NODE *sec)
{
    this->set(first, sec);
}

ECG_EDGE::ECG_EDGE(ECG_NODE *first, ECG_NODE *sec, StreamLine *sl)
{
    this->set(first, sec, sl);
}


// don't delete anything
// ECG will take care of deletion
ECG_EDGE::~ECG_EDGE()
{
    this->nodes[0] = this->nodes[1] = nullptr;
    this->sl = nullptr;
}

void ECG_EDGE::set(ECG_NODE* first, ECG_NODE* sec)
{
    if(first == sec || first == nullptr || sec == nullptr){
        Utility::throwErrorMessage("ECG_EDGE::set: something wrong!");
    }
    this->nodes[0] = first;
    this->nodes[1] = sec;
}

void ECG_EDGE::set(ECG_NODE *first, ECG_NODE *sec, StreamLine *sl)
{
    if(first == sec || first == nullptr || sec == nullptr || sl == nullptr){
        Utility::throwErrorMessage("ECG_EDGE::set: something wrong!");
    }

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
        if(sing!=nullptr) delete sing;
    }
    this->sings.clear();


    for(ECG_NODE* node : this->nodes){
        if(node != nullptr) delete node;
    }
    this->nodes.clear();


    for(ECG_EDGE* e : this->edges){
        if(e!=nullptr) delete e;
    }

    this->edges.clear();


    for(StreamLine* sl : this->sls){
        if(sl != nullptr) delete sl;
    }

    this->sls.clear();
}

void ECG::add_sing(Singularity * sing)
{
    // check if singularity exists in sings already
    if(this->sings_set.find(sing) != this->sings_set.end()) return;
    this->sings_set.insert(sing);
    this->sings.push_back(sing);
}

void ECG::add_node(ECG_NODE * node)
{
    // check if node exists in nodes already
    if(this->nodes_set.find(node) != this->nodes_set.end()) return;
    this->nodes_set.insert(node);
    this->nodes.push_back(node);
}

void ECG::add_edge(ECG_EDGE * ecg_e)
{
    // check if edge exists in edges already
    if(this->edges_set.find(ecg_e) != this->edges_set.end()) return;
    this->edges_set.insert(ecg_e);
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
    srand((unsigned)time(nullptr));

    // for each singularity
    for(ECG_NODE* node : this->nodes){
        Singularity* sing = node->sing;
        vector<StreamLine*> sls;
        // randomly placing unique num_of_seeds seeds around it
        set<TRIPLE> used; // fake triple in c++
        UL cur_num_of_seeds = 0;
        while(cur_num_of_seeds < num_of_seeds){
            // generate a random numebrs between -dist and dist
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
                if(new_cords_tet == nullptr) continue;
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
    for(Singularity* sing : this->sings){
        ECG_NODE* node = new ECG_NODE(sing);
        this->add_node(node);
    }
}


// call this after ECG_NODES are built
// given the streamline seeds, we trace them,
// at any time, if we see a streamline is really close to another singulairty while tracing,
// we constrcuct an directed edge between two ECG nodes and stop tracing
void ECG::build_ECG_EDGES(Mesh *mesh, vector< vector<StreamLine *> > sls_for_all_sings)
{
    for(UL i = 0; i < sls_for_all_sings.size(); i ++){
        // get the reference to the node and streamlines near it
        ECG_NODE* node = this->nodes[i];
        vector<StreamLine*> sls = sls_for_all_sings[i];

        // tracing each streamline for this node
        for(StreamLine* sl : sls){
            bool found = false;
            // while tracing sl, we need to check if the new vertex of the streamline is close to another singularity
            // forward tracing
            {
                const Vertex* vert = sl->seed;
                for(UI j = 0; j < max_num_steps; j++){
                    Tet* tet = vert->tets[0];
                    Vector3d cords = vert->cords;
                    Vector3d newCords = trace_one_dist_step(cords, vert->vels.at(t)); // trace 1 time step
                    double ds[4]; // saving barycentric coordinates
                    Tet* newTet = mesh->inWhichTet(newCords, tet, ds); // find the corresponding tet
                    if(newTet == nullptr) {
                        break; // newTet is nullptr means we couldn't proceed
                    }
                    // interpolate at newCords at time t
                    Vertex* newVert = newTet->get_vert_at(newCords, t, ds); // interpolate new cords in the tet
                    if(newVert == nullptr) Utility::throwErrorMessage("ECG::build_ECG_EDGES: newVert is nullptr!");

                    newVert->add_tet(newTet);
                    sl->fw_verts.push_back(newVert); // new vert into the streamline
                    vert = newVert; // update vert for the next iteration

                    // check if new Vertex is close to any of the singularity
                    ECG_NODE* close_to_node = this->is_close_to_node(newVert->cords);
                    // check if close_to_node exists and check if this node exists in out_nodes
                    if(close_to_node != nullptr  && node != close_to_node && !node->has_outNode(close_to_node)){
                        // the streamline connects node and close_to_node
                        // we should build an directed edge from node to close_to_node
                        ECG_EDGE* edge = new ECG_EDGE(node, close_to_node, sl);
                        node->add_outNode(close_to_node);
                        node->add_outEdge(edge);
                        close_to_node->add_inNode(node);
                        close_to_node->add_inEdge(edge);
                        this->add_edge(edge);
                        this->add_sl(sl);
                        found = true;
                        break; // stop tracing
                    }
                }
            }

            if(found == false && show_ECG_connections){
                // clear all vertices in sl
                sl->clear_fw_verts();
            }

            // backward tracing
            {
                Vertex* vert = sl->seed;
                for(UI j = 0; j < max_num_steps; j++){
                    Tet* tet = vert->tets[0];
                    Vector3d vel = Vector3d( vert->vels.at(t) ) * (- 1.); // -1 means backward
                    Vector3d cords = vert->cords;
                    Vector3d newCords = trace_one_dist_step(cords, vel); // trace 1 time step
                    double ws[4]; // saving barycentric coordinates
                    Tet* newTet = mesh->inWhichTet(newCords, tet, ws); // find the corresponding tet
                    if(newTet == nullptr) {
                        break; // newTet is nullptr means we couldn't proceed
                    }
                    // interpolate at newCords at time t
                    Vertex* newVert = newTet->get_vert_at(newCords, t, ws); // interpolate new cords in the tet
                    if(newVert == nullptr) Utility::throwErrorMessage("ECG::build_ECG_EDGES: newVert is nullptr!");

                    newVert->add_tet(newTet);
                    sl->bw_verts.push_back(newVert); // new vert into the streamline
                    cords = newCords; // update cords for the next iteration
                    vert = newVert;

                    // check if new Vertex is close to any of the singularity
                    ECG_NODE* close_to_node = this->is_close_to_node(newVert->cords);
                    // check if close_to_node exists and check if this node exists in out_nodes
                    if(close_to_node != nullptr  && node != close_to_node  && !node->has_inNode(close_to_node) ){
                        // the streamline connects node and close_to_node
                        // we should build an directed edge from node to close_to_node
                        ECG_EDGE* edge = new ECG_EDGE(close_to_node, node, sl);
                        node->add_inNode(close_to_node);
                        node->add_inEdge(edge);
                        close_to_node->add_outNode(node);
                        close_to_node->add_outEdge(edge);
                        this->add_edge(edge);
                        if(found == false){
                            this->add_sl(sl);
                        }
                        found = true;
                        break; // stop tracing
                    }
                }
            }

            // if found
            if(found == false && show_ECG_connections){
                delete sl;
            }
            else if(found == false && !show_ECG_connections){
                this->add_sl(sl);
            }
        }
    }
}


// returning a vector of nodes that have zero incoming edges
// excluding isolated nodes
const vector<ECG_NODE *> ECG::get_Zero_InDegree_Nodes() const
{
    vector<ECG_NODE*> nodes;
    for(ECG_NODE* node : this->nodes){
        if(node->num_inNodes() == 0 && node->num_outNodes() != 0){
            nodes.push_back(node);
        }
    }
    return nodes;
}


// returning a vector of nodes that have zero outgoing edges
// excluding isolated nodes
const vector<ECG_NODE *> ECG::get_Zero_OutDegree_Nodes() const
{
    vector<ECG_NODE*> nodes;
    for(ECG_NODE* node : this->nodes){
        if(node->num_outNodes() == 0 && node->num_inNodes() != 0){
            nodes.push_back(node);
        }
    }
    return nodes;
}


// returning a vector of nodes that have zero outgoing edges
const vector<ECG_NODE *> ECG::get_isolated_Nodes() const
{
    vector<ECG_NODE*> nodes;
    for(ECG_NODE* node : this->nodes){
        if(node->num_outNodes() == 0 && node->num_inNodes() == 0){
            nodes.push_back(node);
        }
    }
    return nodes;
}


// return the first node if the incoming cords is close to it
// return nullptr if none of the node in ECG is close to this cord
ECG_NODE *ECG::is_close_to_node(const Vector3d &cords) const
{
    for(ECG_NODE* node : this->nodes){
        const Vector3d& node_cords = node->sing->cords;
        if(length(cords, node_cords) <= dist_step_size * 2){
            return node;
        }
    }
    return nullptr;
}

void build_ECGs(vector<Mesh *> meshes)
{
    for(UI i = 0; i < meshes.size(); i++){
        Mesh* mesh = meshes[i];
        qDebug() << "Building ECG for mesh" << i;
        mesh->build_ECG_for_all_t();
        qDebug() << "Building ECG for mesh" << i << "done";
    }
}
