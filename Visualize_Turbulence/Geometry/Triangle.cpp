#include "Triangle.h"

// constructor
Triangle::Triangle()
{
    this->idx = 0;

    // reserve memories
    this->verts.reserve(3); // a face at least has 3 verts
    this->edges.reserve(3); // a face at least has 3 edges
    this->tets.reserve(2);   // a face can be shared by at most 2 tet
}


// desctructor
Triangle::~Triangle()
{
    this->verts.clear();
    this->edges.clear();
    this->tets.clear();
}
