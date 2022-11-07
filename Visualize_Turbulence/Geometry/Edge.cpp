#include "Edge.h"

// constructor
Edge::Edge()
{
    this->idx = -1; // using -1 to indicate the obj is new

    // reserve memories
    this->verts.reserve(2); // an edge should only have 2 verts
}


// destructor
Edge::~Edge()
{
    this->verts.clear();
    this->faces.clear();
    this->tets.clear();
}


int Edge::num_verts(){
    return this->verts.size();
}


int Edge::num_faces(){
    return this->faces.size();
}


int Edge::num_tets(){
    return this->tets.size();
}


void Edge::add_vert(Vertex* v){
    this->verts.push_back(v);
}


void Edge::add_face(Face* f){
    this->faces.push_back(f);
}


void Edge::add_tet(Tet* tet){
    this->tets.push_back(tet);
}

