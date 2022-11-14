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


unsigned long Tet::num_verts(){
    return this->verts.size();
}


unsigned long Tet::num_edges(){
    return this->edges.size();
}


unsigned long Tet::num_tris(){
    return this->tris.size();
}


unsigned long Tet::num_tets(){
    return this->tets.size();
}


void Tet::add_vert(Vertex* v){
    this->verts.push_back(v);
}


void Tet::add_edge(Edge* e){
    this->edges.push_back(e);
}


void Tet::add_triangle(Triangle* f){
    this->tris.push_back(f);
}


void Tet::add_tet(Tet* tet){
    this->tets.push_back(tet);
}
