#include "Vertex.h"

// constructors
Vertex::Vertex()
{
    this->idx = -1; // using -1 to indicate this vertex has not been assigned
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}


Vertex::Vertex( int idx, double x, double y, double z )
{
    this->idx = idx;
    this->x = x;
    this->y = y;
    this->z = z;
}


// destructor
Vertex::~Vertex()
{
    this->edges.clear();
    this->faces.clear();
    this->edges.clear();
}
