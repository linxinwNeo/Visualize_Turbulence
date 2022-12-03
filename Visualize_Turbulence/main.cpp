#include "mainwindow.h"

#include <QApplication>
#include <cstdlib>
#include <set>
#include "Others/GlobalVars.h"

extern ReadFile* file;
extern Mesh* mesh;
extern const QString meshFilePath;
extern const QString dataFilePath;

// seed generation related
extern vector<PathLine*> pathlines;
extern const unsigned int NUM_SEEDS;
extern unsigned int max_num_steps; // currently equal to the number of time frames
extern const double step_size; // currently 1

void read_files();
void place_seeds();
void build_pathlines_from_seeds();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    read_files();

    // place inital seeds
    place_seeds();
    // trace seeds and form pathlines
    build_pathlines_from_seeds();
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
            Point seed = tet->centroid();
            Vertex* seed_v = tet->get_vert_at(&seed, 0); // interpolate this seed at time 0
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
        Vertex* seed = pathline->verts[0];
        Tet* inWhichTet = seed->tets[0];
    }
}
