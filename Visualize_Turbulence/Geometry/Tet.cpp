#include "Tet.h"


Tet::Tet()
{
    this->idx = -1; // use -1 to indicate the obj is new

    // reserve memories
    this->verts.reserve(4); // a tet can only have exact 4 vertices
    this->edges.reserve(4); // a tet can only have exact 4 edges
    this->tris.reserve(4); // a tet can only have exact 4 faces
    this->tets.reserve(4);  // a tet can only have 4 neighbor tets
}

Tet::~Tet()
{
    this->verts.clear();
    this->edges.clear();
    this->tris.clear();
    this->tets.clear();
}

