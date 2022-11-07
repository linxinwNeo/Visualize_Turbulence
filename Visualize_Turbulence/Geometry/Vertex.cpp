#include "Geometry/Vertex.h"

// constructors
Vertex::Vertex()
{
    this->idx = -1; // using -1 to indicate this vertex has not been assigned
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}


Vertex::Vertex( double x, double y, double z )
{
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


int Vertex::num_edges(){
    return this->edges.size();
}


int Vertex::num_faces(){
    return this->faces.size();
}


int Vertex::num_tets(){
    return this->tets.size();
}


void Vertex::add_edge(Edge* e){
    this->edges.push_back(e);
}


void Vertex::add_face(Face* f){
    this->faces.push_back(f);
}


void Vertex::add_tet(Tet* tet){
    this->tets.push_back(tet);
}
