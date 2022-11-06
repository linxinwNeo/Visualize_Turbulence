#include "Edge.h"

// constructor
Edge::Edge()
{
    this->idx = -1; // using -1 to indicate the obj is new
}


// destructor
Edge::~Edge()
{
    this->verts.clear();
    this->faces.clear();
    this->tets.clear();
}
