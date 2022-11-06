#include "Face.h"

// constructor
Face::Face()
{
    this->idx = -1;
}


// desctructor
Face::~Face()
{
    this->verts.clear();
    this->edges.clear();
    this->tets.clear();
}
