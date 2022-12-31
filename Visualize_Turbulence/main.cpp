#include <queue>
#include <QApplication>
#include <cstdlib>
#include <set>
#include <iostream>
#include "Eigen/Dense"

#include "FileLoader/ReadFile.h"
#include "Others/Utilities.h"
#include "Geometry/Mesh.h"
#include "Surfaces/Isosurface.h"
#include "Lines/StreamLine.h"
#include "Others/ColorTable.h"
#include "mainwindow.h"

typedef pair<double, Tet*> qPair;

// the obj that used to read the file
ReadFile* file = NULL;


// the data of the file is saved orderly in mesh obj
vector<Mesh*> meshes;


// the file path, should be entered before starting the program
// TODO dynamiclly import files
const QString filePathPrefix = "/Users/linxinw/Desktop/simulation data/";

const QString meshFilePath1 = filePathPrefix + "no_boundary_slow_mesh.txt";
const QString dataFilePath1 = filePathPrefix + "no_boundary_slow_data.txt";

const QString meshFilePath2 = filePathPrefix + "no_boundary_medium_mesh.txt";
const QString dataFilePath2 = filePathPrefix + "no_boundary_medium_data.txt";

const QString meshFilePath3 = filePathPrefix + "no_boundary_fast_mesh.txt";
const QString dataFilePath3 = filePathPrefix + "no_boundary_fast_data.txt";

const QString meshFilePath4 = filePathPrefix + "with_boundary_slow_mesh.txt";
const QString dataFilePath4 = filePathPrefix + "with_boundary_slow_data.txt";

const QString meshFilePath5 = filePathPrefix + "with_boundary_medium_mesh.txt";
const QString dataFilePath5 = filePathPrefix + "with_boundary_medium_data.txt";

const QString meshFilePath6 = filePathPrefix + "with_boundary_fast_mesh.txt";
const QString dataFilePath6 = filePathPrefix + "with_boundary_fast_data.txt";

const QString meshFilePath7 = filePathPrefix + "mesh.txt";
const QString dataFilePath7 = filePathPrefix + "data.txt";


// boolean variables used to enable orbit control
bool LeftButtonDown = false;
bool MiddleButtonDown = false;
bool RightButtonDown = false;

// streamlines
bool show_streamlines = true;
bool tracing_streamlines_from_seed = false;
bool tracing_streamlines_from_critical_pts =true;

bool show_pathlines = false;
bool show_isosurfaces = false;

bool show_boundary_wireframe = false;
bool show_axis = true;
bool show_opage_boundary_tris = true;
bool show_critical_pts = true;

const unsigned int NUM_SEEDS = 100;
const unsigned int max_num_steps = 400;
const double dist_step_size = 0.003;
const UI frames_per_sec = 1; // frames per sec
const double time_step_size = ((double)1.)/(double)frames_per_sec; // sec for each frame
//const double time_step_size = 0.1;
const unsigned int max_num_recursion = 4;
const double zero_threshold = 1e-17;

// surface_level is defined to be the voriticity
const double surface_level_ratio = 0.02;
unordered_map<double, double> surface_level_vals;

// arrow parameters
const double cone_base_radius=0.01;
const double cone_height=cone_base_radius*2;
const double cylinder_height = cone_height*2;
const double cylinder_radius = cone_base_radius/2;
const int slices = 5;
const double arrow_color[] = {1, 0, 0};

ColorTable CT;

// rendering parameters
const double boundary_tri_alpha = 0.1;

// main functions
void read_files();

void replace_velocity(){
    Mesh* mesh = meshes[0];
    double time = 0.;
    while(time < mesh->num_time_steps - 1){
        for(Vertex* v : mesh->verts){
            if(time == 0) v->cords.entry[2] += 0.25;
            Vector3d cords = v->cords;
            int scale = 2;
            v->vels[time]->set(scale*cords.x(), scale*cords.y(), scale*cords.z());
//            normalize(*v->vels[time]);
        }
        time += time_step_size;
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // read files and build mesh
    read_files();

    // replace velocity vectors with analytical equations
    meshes[0]->num_time_steps =10;
    //replace_velocity();

    // constucting the data for rendering
    if(show_isosurfaces)
        construct_isosurfaces();

    if(show_critical_pts)
        capture_critical_pts(meshes);



    if(show_streamlines)
        tracing_streamlines();


    MainWindow w;
    w.show();

    return a.exec();
}


void read_files(){
    file = new ReadFile( meshFilePath7, dataFilePath7 );
    meshes.push_back( file->mesh );
    delete file;

//    file = new ReadFile( meshFilePath2, dataFilePath2);
//    meshes.push_back( file->mesh );
//    delete file;

//    file = new ReadFile( meshFilePath3, dataFilePath3);
//    meshes.push_back( file->mesh );
//    delete file;

//    file = new ReadFile( meshFilePath4, dataFilePath4);
//    meshes.push_back( file->mesh );
//    delete file;

//    file = new ReadFile( meshFilePath5, dataFilePath5);
//    meshes.push_back( file->mesh );
//    delete file;

//    file = new ReadFile( meshFilePath6, dataFilePath6);
//    meshes.push_back( file->mesh );
//    delete file;
}
