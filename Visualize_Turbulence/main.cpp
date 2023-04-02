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
bool tracing_streamlines_from_seed = true;

bool show_pathlines = false;
bool show_isosurfaces = false;

bool show_boundary_wireframe = false;
bool show_axis = true;
bool show_opage_boundary_tris = true;
bool build_ECG = true;
bool show_ECG_connections = false;
bool show_ECG_edge_constructions = false;
bool show_seeds = true;
bool show_fixedPts = true;
bool show_tets_with_fixedPts = true;

const double h = 1e-3;
const UI NUM_SEEDS = 50;
const UI max_num_steps = 500;
//const UI NUM_SEEDS_for_Limit = 10;
//const UI max_num_steps_for_Limit = 100;
const double dist_step_size = 1e-2;
const UI frames_per_sec = 1; // frames per sec
const double time_step_size = ((double)1.)/(double)frames_per_sec; // sec for each frame
//const double time_step_size = 0.1;
const UI max_num_recursion = 4;
const double zero_threshold = 1e-15;


// surface_level is defined to be the voriticity
const double surface_level_ratio = 0.02;
unordered_map<double, double> surface_level_vals;

// arrow parameters
const double cone_base_radius = 0.01;
const double cone_height= cone_base_radius*2;
const double cylinder_height = cone_height*2;
const double cylinder_radius = cone_base_radius/2;
const int slices = 3;
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
            if(time == 0) {
                v->cords.entry[2] += 0.25;
            }

            Vector3d cords = v->cords;
            float s = 1;
            double x = cords.x();
            double y = cords.y();
            double z = cords.z();
            v->vels[time]->set(s*x, s*y, 0);
//            qDebug() <<length(*v->vels[time]);
            normalize(*v->vels[time]);
        }

        // update centroid
        if(time == 0){
            for(Tet* tet : mesh->tets){
                tet->center = tet->centroid();
            }
        }

        time += time_step_size;
    }
}


void test_fixedPtDetection_Robust(){
    Mesh* mesh = meshes[0];
    Tet* tet = mesh->tets[0];
    Vertex* v1 = tet->verts[0];
    Vertex* v2 = tet->verts[1];
    Vertex* v3 = tet->verts[2];
    Vertex* v4 = tet->verts[3];

    // replace coordinates
    v1->cords = Vector3d(sqrt(8./9.), 0, -1./3.);
    v2->cords = Vector3d(-sqrt(2./9.), sqrt(2./3.), -1./3.);
    v3->cords = Vector3d(-sqrt(2./9.), -sqrt(2./3.), -1./3.);
    v4->cords = Vector3d(0, 0, 1);

    // replace velocity
    int s = 0.1;
    for(Vertex* v : tet->verts){
        v->vels[0] = new Vector3d(s*v->x(), s*v->y(), s*v->z());
//        qDebug() << v->vels[0]->x() << v->vels[0]->y() << v->vels[0]->z();
    }

    qDebug() << " result: " << mesh->has_fixedPt_Robust(tet, 0);

    exit(10);
}

void testing_DetectfixedPt_subd(){
    Mesh* mesh = meshes[0];
    Tet* tet = mesh->tets[0];
    Vertex* v1 = tet->verts[0];
    Vertex* v2 = tet->verts[1];
    Vertex* v3 = tet->verts[2];
    Vertex* v4 = tet->verts[3];

    v1->cords = Vector3d(sqrt(8./9.), 0, -1./3.);
    v2->cords = Vector3d(-sqrt(2./9.), sqrt(2./3.), -1./3.);
    v3->cords = Vector3d(-sqrt(2./9.), -sqrt(2./3.), -1./3.);
    v4->cords = Vector3d(0, 0.01, 1);

    for(Vertex* v : tet->verts){
//        v->vels[0] = new Vector3d(v->x(), v->y(), v->z()/10000000000000000000000000000000000000000000.);
         v->vels[0] = new Vector3d(v->x(), v->y(), 0);
//        qDebug() << length(*v->vels[0]);
        normalize(*v->vels[0]);
    }

    double ws[4];

    vector<Vector3d> sings;


    for(Vector3d sing : sings){
        qDebug() << sing.x() << sing.y() << sing.z();
        // calculate the jacobian matrix
        Singularity* singularity = new Singularity();
        singularity->Jacobian = tet->calc_Jacobian(sing, 0); // calculate the jacobian for this sing
        singularity->classify_this(); // classify the type of the singularity
        qDebug() << singularity->get_type();
    }

    exit(1);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // read files and build mesh
    read_files();

    // replace velocity vectors with analytical equations
//    meshes[0]->num_time_steps = 5;
//    replace_velocity();

//    testing_subdivision();
//    test_fixedPtDetection_Robust();

    // constucting the data for rendering
    if(show_isosurfaces)
        construct_isosurfaces();

    if(build_ECG)
        build_ECGs(meshes);

    if(show_streamlines)
        tracing_streamlines();


    MainWindow w;
    w.show();

    return a.exec();
}


void read_files(){
    file = new ReadFile( meshFilePath3, dataFilePath3 );
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
