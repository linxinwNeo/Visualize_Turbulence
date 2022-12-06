#include "Lines/StreamLine.h"
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
// we calculate streamlines for each original time steps, so [0] means the strealines for the first time step
vector< vector<StreamLine*> > streamlines_for_all_t;
const unsigned int NUM_SEEDS = 100;
const unsigned int max_num_steps = 2000;
const double time_step_size = 0.5;
const double dist_step_size = 0.0005;
// when trace a point, we don't care the position that may be too far away
const double dist_scale = 10.;

// arrow parameters
const double cone_base_radius=0.01;
const double cone_height=cone_base_radius*2;
const double cylinder_height = cone_height*2;
const double cylinder_radius = cone_base_radius/2;
const int slices = 5;
const double arrow_color[] = {1, 0, 0};
ColorTable CT;

void read_files();
void place_seeds(vector<PathLine*>&);
void place_seeds(vector< vector<StreamLine*> > &);
void build_pathlines_from_seeds();
Vector3d trace_one_time_step(const Vector3d& start_cords, const Vector3d& vel);
Vector3d trace_one_dist_step(const Vector3d& start_cords, const Vector3d& vel);
Tet* inWhichTet(const Vector3d& target_pt, Tet* prev_tet, double ds[4]);
vector<UL> generate_unique_random_Tet_idx();
void build_streamlines_from_seeds();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // read files and build mesh
    read_files();

    // place inital seeds
    place_seeds(streamlines_for_all_t);
    // trace seeds and form pathlines
    // mainwindow.cpp will clear the memory of pathlines and streamlines
    build_streamlines_from_seeds();

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
void place_seeds(vector<PathLine*>& pls)
{
    srand((unsigned) time(NULL));

    set<UL> seeded_tets;
    unsigned int cur_num_seeds = 0;

    pls.reserve(NUM_SEEDS);

    while(cur_num_seeds < NUM_SEEDS)
    {
        int random_idx = rand() % mesh->num_tets();
        if( seeded_tets.find(random_idx) != seeded_tets.end() ) continue;
        else{
            seeded_tets.insert(random_idx);
            Tet* tet = mesh->tets[random_idx];
            PathLine* PL = new PathLine();
            Vector3d seed = tet->centroid();
            Vertex* seed_v = tet->get_vert_at(seed, 0.); // interpolate this seed at time 0
            PL->verts.push_back( seed_v );
            pls.push_back(PL);
            cur_num_seeds ++;
        }
    }
}


void place_seeds(vector< vector<StreamLine*> >& all_sls)
{
    all_sls.reserve(mesh->num_time_steps);
    // make sure we are using same seeds every time step
    vector<UL> seeds = generate_unique_random_Tet_idx();

    // for each time step, place streamline seeds
    for(UI i = 0; i < mesh->num_time_steps; i++)
    {
        vector<StreamLine*> sls; // create a vector of streamlines to store seeds
        sls.reserve(NUM_SEEDS); // we have num_seeds streamlines for each time step

        unsigned int cur_num_seeds = 0;
        while(cur_num_seeds < NUM_SEEDS)
        {
            // set up the streamline
            StreamLine* SL = new StreamLine();
            SL->fw_verts.reserve(max_num_steps);
            SL->bw_verts.reserve(max_num_steps);
            SL->time = (double) i;

            UL tet_idx = seeds[cur_num_seeds];
            Tet* rdm_tet = mesh->tets[tet_idx];
            Vector3d center_pt = rdm_tet->centroid();
            Vertex* center_vert = rdm_tet->get_vert_at(center_pt, (double)i);
            SL->set_seed( center_vert );
            cur_num_seeds ++;
            sls.push_back(SL);
        }

        all_sls.push_back(sls);
    }
}


vector<UL> generate_unique_random_Tet_idx()
{
    srand((unsigned) time(NULL));
    set<UL> seeded_tets;
    unsigned int cur_num_seeds = 0;
    while(cur_num_seeds < NUM_SEEDS)
    {
        int random_idx = rand() % mesh->num_tets();
        if( seeded_tets.find(random_idx) != seeded_tets.end() ) continue;
        else{
            seeded_tets.insert(random_idx);
            cur_num_seeds ++;
        }
    }

    vector<UL> seeds;
    seeds.reserve(NUM_SEEDS);
    for(UL idx : seeded_tets){
        seeds.push_back(idx);
    }
    return seeds;
}


// now every streamline at any time has a seed point as a starting point, we want to calculate their trajectory individually
void build_streamlines_from_seeds(){
    // for each time step
    double cur_time = 0;
    for( vector<StreamLine*>& sls : streamlines_for_all_t ){
        // for each (the beginning of) trajectory
        for( StreamLine* sl : sls ){
            // forward tracing
            {
                Vertex* seed_vert = sl->seed;
                Vector3d cords = seed_vert->cords;
                Tet* tet = seed_vert->tets[0];
                for(UI i = 0; i < max_num_steps; i++){
                    Vector3d newCords = trace_one_dist_step(cords, seed_vert->vels.at(cur_time)); // trace 1 time step
                    double ds[4]; // saving barycentric coordinates
                    Tet* newTet = inWhichTet(newCords, tet, ds); // find the corresponding tet
                    if(newTet == NULL) {
                        break; // newTet is null means we couldn't proceed
                    }
                    // interpolate vertex's vel, vor, mu at time t
                    Vertex* newVert = newTet->get_vert_at(newCords, cur_time); // interpolate new cords in the tet
                    if(newVert == NULL) throwErrorMessage("build_pathlines_from_seeds: newVert is NULL!");
                    newVert->add_tet(newTet);
                    sl->fw_verts.push_back(newVert); // new vert into the pathline
                    cords = newCords; // update cords for the next iteration
                    seed_vert = newVert;
                }
            }

            // backward tracing
            {
                Vertex* vert = sl->seed;
                Tet* tet = vert->tets[0];
                for(UI i = 0; i < max_num_steps; i++){
                    Vector3d vel = Vector3d( vert->vels.at(cur_time) ) * - 1.; // -1 means backward
                    Vector3d cords = vert->cords;
                    Vector3d newCords = trace_one_dist_step(cords, vel); // trace 1 time step
                    double ds[4]; // saving barycentric coordinates
                    Tet* newTet = inWhichTet(newCords, tet, ds); // find the corresponding tet
                    if(newTet == NULL) {
                        break; // newTet is null means we couldn't proceed
                    }
                    // interpolate vertex's vel, vor, mu at time t
                    Vertex* newVert = newTet->get_vert_at(newCords, cur_time); // interpolate new cords in the tet
                    if(newVert == NULL) throwErrorMessage("build_pathlines_from_seeds: newVert is NULL!");
                    newVert->add_tet(newTet);
                    sl->bw_verts.push_back(newVert); // new vert into the pathline
                    vert = newVert;
                }
            }
        }
        cur_time += 1.;
        break;
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
        for(UI i = 0; i < max_num_steps && cur_time < mesh->num_time_steps - 1 - time_step_size; i++){
            Vector3d newCords = trace_one_time_step(cords, seed_vert->vels.at(cur_time)); // trace 1 time step
            double ds[4];
            Tet* newTet = inWhichTet(newCords, tet, ds); // find the corresponding tet
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
// ds contains barycentric coordinate of this pt in that tet if found
Tet* inWhichTet(const Vector3d& target_pt, Tet* prev_tet, double ds[4])
{
    // it only breaks if we found the target
    while(!prev_tet->is_pt_in2(target_pt, ds)){
        unsigned int min_idx; double min_val;
        array_min(ds, 4, min_idx, min_val);
        if(min_val > 0){ // the pt is in prev_tet
            return prev_tet;
        }
        // the pt is not in prev_tet
        // we should move to the neighbor whose barycentric coordinate is smallest.
        Vertex* min_vert = prev_tet->verts[min_idx];
        Triangle* exit_tri = NULL;
        for(unsigned int i = 0; i<prev_tet->num_tris(); i++ ){
            // find the triangle that does not have this min_vert
            if(!prev_tet->tris[i]->has_vert(min_vert)){
                exit_tri = prev_tet->tris[i];
            }
        }
        // if we couldn't find exit_tri or the exit_tri is a dead end, we couldn't proceed
        if(exit_tri == NULL || exit_tri->is_boundary){
            if(exit_tri == NULL) qDebug() << "exit_tri is null!";
            return NULL;
        }
        // then we set up the next iteration
        for(int i = 0; i < 2; i++){
            if(exit_tri->tets[i] != prev_tet){
                prev_tet = exit_tri->tets[i];
                break;
            }
        }
    }
    return prev_tet;
}


// tracing the start_cords one time step
// vel is the velocity vector
// return the resultant cords
Vector3d trace_one_time_step(const Vector3d& start_cords, const Vector3d& vel)
{
    return start_cords + vel*time_step_size;
}


Vector3d trace_one_dist_step(const Vector3d& start_cords, const Vector3d& vel)
{
    Vector3d v = Vector3d(vel);
    normalize(v);
    return start_cords + v*dist_step_size;
}
