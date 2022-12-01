#ifndef GLOBALVARS_H
#define GLOBALVARS_H

// we place global variables in this file
#include <QString>
#include "FileLoader/ReadFile.h"
#include "Lines/PathLine.h"

// the obj that used to read the file
ReadFile* file = NULL;


// the data of the file is saved orderly in mesh obj
Mesh* mesh = NULL;


// the file path, should be entered before starting the program
// TODO dynamiclly import files
const QString meshFilePath = "/Users/linxinw/Desktop/slow_mesh.txt";
const QString dataFilePath = "/Users/linxinw/Desktop/slow_data.txt";

// boolean variables used to enable orbit control
bool LeftButtonDown = false;
bool MiddleButtonDown = false;
bool RightButtonDown = false;

// calculated pathlines used for animation
// each element in pathlines is a trajectory over space and time
vector<PathLine*> pathlines;
const unsigned int NUM_SEEDS = 10;
unsigned int max_num_steps = 60;
const double step_size = 1;

#endif // GLOBALVARS_H
