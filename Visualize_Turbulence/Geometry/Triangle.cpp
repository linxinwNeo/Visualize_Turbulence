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


unsigned long Triangle::num_verts(){
    return this->verts.size();
}


unsigned long Triangle::num_edges(){
    return this->edges.size();
}


unsigned long Triangle::num_tets(){
    return this->tets.size();
}


void Triangle::add_vert(Vertex* v){
    this->verts.push_back(v);
}


void Triangle::add_edge(Edge* e){
    this->edges.push_back(e);
}


void Triangle::add_tets(Tet* tet){
    this->tets.push_back(tet);
}

