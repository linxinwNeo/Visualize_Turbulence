#include <queue>
#include <QApplication>
#include <cstdlib>
#include <set>

#include "FileLoader/ReadFile.h"
#include "Others/Utilities.h"
#include "Geometry/Mesh.h"
#include "Surfaces/Isosurface.h"
#include "Lines/StreamLine.h"
#include "Others/ColorTable.h"
#include "Lines/PathLine.h"
#include "mainwindow.h"

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
// boolean variables used to enable modes
bool show_streamlines = true;
bool show_pathlines = false;
bool show_isosurfaces = true;
bool show_boundary_wireframe = false;
bool show_axis = false;
bool show_opage_boundary_tris = true;

// calculated pathlines used for animation
// each element in pathlines is a trajectory over space and time
vector<PathLine*> pathlines;
// we calculate streamlines for each original time steps, so [0] means the strealines for the first time step
unordered_map< double, vector<StreamLine*> > streamlines_for_all_t;
unordered_map< double, Isosurface*> isosurfaces_for_all_t;
const unsigned int NUM_SEEDS = 50;
const unsigned int max_num_steps = 400;
const double dist_step_size = 0.003;
const UI frames_per_sec = 10; // frames per sec
const double time_step_size = ((double)1.)/(double)frames_per_sec; // sec for each frame
//const double time_step_size = 0.1;

// surface_level is defined to be the voriticity
const double surface_level_ratio = 0.9;
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // read files and build mesh
    read_files();

    // constucting the data for rendering
    if(show_streamlines)
        tracing_streamlines();

    if(show_isosurfaces)
        constuct_isosurfaces();

    MainWindow w;
    w.show();

    return a.exec();
}


void read_files(){
    file = new ReadFile( meshFilePath, dataFilePath );
    mesh = file->mesh;
}
