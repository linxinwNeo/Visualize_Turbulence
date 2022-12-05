#include "Others/ColorTable.h"
#include "mainwindow.h"

#include <queue>
#include <QApplication>
#include <cstdlib>
#include <set>

#include "FileLoader/ReadFile.h"
#include "Others/Utilities.h"
#include "Geometry/Mesh.h"
#include "Lines/PathLine.h"

typedef pair<double, Tet*> qPair;

// the obj that used to read the file
ReadFile* file = NULL;


// the data of the file is saved orderly in mesh obj
Mesh* mesh = NULL;


// the file path, should be entered before starting the program
// TODO dynamiclly import files
const QString meshFilePath = "/Users/linxinw/Desktop/fast_mesh.txt";
const QString dataFilePath = "/Users/linxinw/Desktop/fast_data.txt";

// boolean variables used to enable orbit control
bool LeftButtonDown = false;
bool MiddleButtonDown = false;
bool RightButtonDown = false;

// calculated pathlines used for animation
// each element in pathlines is a trajectory over space and time
vector<PathLine*> pathlines;
//const unsigned int NUM_SEEDS = 1;
const unsigned int max_num_steps = 60;
const double time_step_size = 1;
// when trace a point, we don't care the position that may be too far away
const double dist_scale = 5.;

// arrow parameters
const double cone_base_radius=0.01;
const double cone_height=cone_base_radius*2;
const double cylinder_height = cone_height*2;
const double cylinder_radius = cone_base_radius/2;
const int slices = 10;
const double arrow_color[] = {1, 0, 0};
ColorTable CT;


void read_files();
void place_seeds();
void build_pathlines_from_seeds();
Vector3d trace_one_step(const Vector3d& start_cords, const Vector3d& vel);
Tet* inWhichTet(const Vector3d& target, Tet* prev_tet, double moved_dist);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    read_files();

    // place inital seeds
    place_seeds();
    // trace seeds and form pathlines
    // mainwindow.cpp will clear the memory of pathlines
    build_pathlines_from_seeds();


//    PathLine* pl = pathlines[0];
//    Vertex*v = pl->verts[0];
//    qDebug() << v->cords_str();
//    qDebug() << v->vel_str(0);
//    Vertex*v2 = pl->verts[1];
//    qDebug() << v2->cords_str();
//    qDebug() << v2->vel_str(time_step_size);


    MainWindow w;
    w.show();

    return a.exec();
}


void read_files(){
    file = new ReadFile( meshFilePath, dataFilePath );
    mesh = file->mesh;
}


// we try to place seeds randomly in the domain
// for a random tet, if we choose to place a seed,
// a seed will be generated in the center of that tet
// we set number of seeds to be generated
void place_seeds()
{
    srand((unsigned) time(NULL));

    set<UL> seeded_tets;
    unsigned int cur_num_seeds = 0;

    pathlines.reserve(40000);

//    while(cur_num_seeds < NUM_SEEDS)
//    {
//        int random_idx = rand() % mesh->num_tets();
//        if( seeded_tets.find(random_idx) != seeded_tets.end() ) continue;
//        else{
//            seeded_tets.insert(random_idx);
//            Tet* tet = mesh->tets[random_idx];
//            PathLine* PL = new PathLine();
//            Vector3d seed = tet->centroid();
//            Vertex* seed_v = tet->get_vert_at(seed, 0.); // interpolate this seed at time 0
//            PL->verts.push_back( seed_v );
//            pathlines.push_back(PL);
//            cur_num_seeds ++;
//        }
//    }


    for(Tet* tet : mesh->tets)
    {
            PathLine* PL = new PathLine();
            Vector3d seed = tet->centroid();
            Vertex* seed_v = tet->get_vert_at(seed, 0.); // interpolate this seed at time 0
            PL->verts.push_back( seed_v );
            pathlines.push_back(PL);
    }
}


// now every pathline has a seed point, we want to calculate their path individually
void build_pathlines_from_seeds(){
    // for each pathline, we calculate their trajectories
    for( PathLine* pathline : pathlines ){
        Vertex* seed_vert = pathline->verts[0];
        Vector3d cords = seed_vert->cords;
        Tet* tet = seed_vert->tets[0];

        /* tracing the seed n times
         *      trace 1 step, calculate the new cords
         *      find the corresponding tet
         *      interpolate new cords in the tet
         *      add new vert into the pathline
        */
        double cur_time = 0;
        double moving_dist = 0;
        for(UI i = 0; i < max_num_steps && cur_time < mesh->num_time_steps - 1 - time_step_size; i++){
            Vector3d newCords = trace_one_step(cords, seed_vert->vels.at(cur_time)); // trace 1 time step
            moving_dist = length(cords, newCords); // calculate how long we moved
            Tet* newTet = inWhichTet(newCords, tet, moving_dist); // find the corresponding tet
            if(newTet == NULL) break; // newTet is null means we couldn't proceed
            cur_time += time_step_size;

            // interpolate vertex's vel, vor, mu at time t
            Vertex* newVert = newTet->get_vert_at(newCords, cur_time); // interpolate new cords in the tet
            if(newVert == NULL) throwErrorMessage("build_pathlines_from_seeds: newVert is NULL!");
            newVert->add_tet(newTet);
            pathline->verts.push_back(newVert); // new vert into the pathline
            cords = newCords; // update cords for the next iteration
            seed_vert = newVert;
        }
    }

    qDebug() << "num of pathlines: " << pathlines.size();
}


// target is the vertex and we are interested in which tet it is in
// prev_tet is the tet that contains the previous vertex. we should find the tet of target by using the neighbors
// of the start_tet.
Tet* inWhichTet(const Vector3d& target, Tet* prev_tet, double moved_dist)
{
    Tet* targetTet = NULL;
    // create a pq to save the 'closest' tets
    priority_queue< qPair, vector<qPair>, std::greater<qPair> > pq; // pop the smallest weight
    set<Tet*> used; // keep track of used tets
    pq.push( {0., prev_tet} ); // place the first tet into the pq
    used.insert(prev_tet);

    while(!pq.empty())
    {
        qPair p = pq.top(); pq.pop();
        Tet* whichTet = p.second;
        // check if this Tet is the tet we are looking for
        // if yes, process it and exit the while loop
        if(whichTet->is_pt_in(target)){
            targetTet = whichTet;
            break;
        }

        // place the neighbors of tets into the pq which the weights of the distance between the seed to them
        for( Tet* nbTet : whichTet->tets ){
            if(is_in_set(used, nbTet)) continue; // don't add tet that has been used
            double dist = length(target, nbTet->centroid());
            if(dist > dist_scale * moved_dist) continue; // don't add tet that is really far away
            pq.push( {dist, nbTet} );
            used.insert( nbTet );
        }
    }

    used.clear();
    return targetTet;
}


// tracing the start_cords one time step
// vel is the velocity vector
// return the resultant cords
Vector3d trace_one_step(const Vector3d& start_cords, const Vector3d& vel)
{
    return start_cords + vel*time_step_size;
}
