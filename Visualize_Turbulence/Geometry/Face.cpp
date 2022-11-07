#include "Face.h"

// constructor
Face::Face()
{
    this->idx = -1;

    // reserve memories
    this->verts.reserve(3); // a face at least has 3 verts
    this->edges.reserve(3); // a face at least has 3 edges
    this->tets.reserve(2);   // a face can be shared by at most 2 tet
}


// desctructor
Face::~Face()
{
    this->verts.clear();
    this->edges.clear();
    this->tets.clear();
}


int Face::num_verts(){
    return this->verts.size();
}


int Face::num_edges(){
    return this->edges.size();
}


int Face::num_tets(){
    return this->tets.size();
}


void Face::add_vert(Vertex* v){
    this->verts.push_back(v);
}


void Face::add_edge(Edge* e){
    this->edges.push_back(e);
}


void Face::add_tets(Tet* tet){
    this->tets.push_back(tet);
}

