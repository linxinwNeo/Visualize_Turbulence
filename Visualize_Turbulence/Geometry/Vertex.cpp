#include "Geometry/Vertex.h"

// destructor
Vertex::~Vertex()
{
    this->edges.clear();
    this->faces.clear();
    this->edges.clear();
    for(auto& v : this->vels){
        if (v != NULL){
            delete v;
        }
    }
    this->vels.clear();

    for(auto& v : this->vors){
        if (v != NULL){
            delete v;
        }
    }
    this->vors.clear();
}


// copy everything, shallow copy
Vertex* Vertex::clone() const
{
    Vertex* newVert = new Vertex();
    newVert->cords = this->cords;
    newVert->vels = this->vels;
    newVert->vors = this->vors;
    newVert->mus = this->mus;

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
