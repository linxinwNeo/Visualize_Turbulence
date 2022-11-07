#include "Tet.h"


Tet::Tet()
{
    this->idx = -1; // use -1 to indicate the obj is new

    // reserve memories
    this->verts.reserve(4); // a tet can only have exact 4 vertices
    this->edges.reserve(4); // a tet can only have exact 4 edges
    this->faces.reserve(4); // a tet can only have exact 4 faces
    this->tets.reserve(4);  // a tet can only have 4 neighbor tets
}

Tet::~Tet()
{
    this->verts.clear();
    this->edges.clear();
    this->faces.clear();
    this->tets.clear();
}


int Tet::num_verts(){
    return this->verts.size();
}


int Tet::num_edges(){
    return this->edges.size();
}


int Tet::num_faces(){
    return this->faces.size();
}


int Tet::num_tets(){
    return this->tets.size();
}


void Tet::add_vert(Vertex* v){
    this->verts.push_back(v);
}


void Tet::add_edge(Edge* e){
    this->edges.push_back(e);
}


void Tet::add_face(Face* f){
    this->faces.push_back(f);
}


void Tet::add_tet(Tet* tet){
    this->tets.push_back(tet);
}
