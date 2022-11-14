#include "Geometry/Vertex.h"

// constructors
Vertex::Vertex()
{
    this->idx = 0;
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}


Vertex::Vertex( double x, double y, double z )
{
    this->idx = 0;
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


unsigned long Vertex::num_edges(){
    return this->edges.size();
}


unsigned long Vertex::num_faces(){
    return this->faces.size();
}


unsigned long Vertex::num_tets(){
    return this->tets.size();
}


void Vertex::add_edge(Edge* e){
    this->edges.push_back(e);
}


void Vertex::add_face(Triangle* f){
    this->faces.push_back(f);
}


void Vertex::add_tet(Tet* tet){
    this->tets.push_back(tet);
}


// copy everything, shallow copy
Vertex* Vertex::clone()
{
    Vertex* newVert = new Vertex(this->x, this->y, this->z);

    // copy edges
    newVert->edges.reserve( this->edges.size() );
    for( auto & edge : this->edges  ){
        newVert->edges.push_back( edge );
    }

    // copy faces
    newVert->faces.reserve( this->faces.size() );
    for( auto & face : this->faces  ){
        newVert->faces.push_back( face );
    }

    // copy tets
    newVert->tets.reserve( this->tets.size() );
    for( auto & tet : this->tets  ){
        newVert->tets.push_back( tet );
    }
    return newVert;
}


