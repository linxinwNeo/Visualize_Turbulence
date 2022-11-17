#ifndef GLOBALVARS_H
#define GLOBALVARS_H

// we place global variables in this file
#include <QString>
#include "FileLoader/ReadFile.h"

// the obj that used to read the file
ReadFile* file = NULL;


// the data of the file is saved orderly in mesh obj
Mesh* mesh = NULL;


// the file path, should be entered before starting the program
// TODO dynamiclly import files
const QString meshFilePath = "/Users/linxinw/Desktop/mesh.txt";
const QString dataFilePath = "/Users/linxinw/Desktop/data.txt";

// boolean variables used to enable orbit control
bool LeftButtonDown = false;
bool MiddleButtonDown = false;
bool RightButtonDown = false;

#endif // GLOBALVARS_H
