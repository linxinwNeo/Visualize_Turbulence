#include "mainwindow.h"

#include <queue>
#include <QApplication>
#include <cstdlib>
#include <set>
#include "Others/GlobalVars.h"
#include "Others/Utility_functions.h"

typedef pair<double, Tet*> qPair;

extern ReadFile* file;
extern Mesh* mesh;
extern const QString meshFilePath;
extern const QString dataFilePath;

// seed generation related
extern vector<PathLine*> pathlines;
extern const unsigned int NUM_SEEDS;
extern const unsigned int max_num_steps;
extern const double time_step_size;
extern const double dist_scale;

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
    build_pathlines_from_seeds();
    for(PathLine* PL : pathlines){
        qDebug() << PL->num_verts();
    }
    // mainwindow.cpp will clear the memory

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

    pathlines.reserve(NUM_SEEDS);

    while(cur_num_seeds < NUM_SEEDS){
        int random_idx = rand() % mesh->num_tets();
        if( seeded_tets.find(random_idx) != seeded_tets.end() ) continue;
        else{
            seeded_tets.insert(random_idx);
            Tet* tet = mesh->tets[random_idx];
            PathLine* PL = new PathLine();
            Vector3d seed = tet->centroid();
            Vertex* seed_v = tet->get_vert_at(seed, 0.); // interpolate this seed at time 0
            PL->verts.push_back( seed_v );
            pathlines.push_back(PL);
            cur_num_seeds ++;
        }
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
        for(UI i = 0; i < max_num_steps && cur_time < mesh->num_time_steps - time_step_size; i++){
            qDebug() << cur_time;
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
