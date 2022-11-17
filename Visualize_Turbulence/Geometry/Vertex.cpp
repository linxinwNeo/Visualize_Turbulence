#include "Geometry/Vertex.h"

// copy everything, shallow copy
Vertex* Vertex::clone()
{
    Vertex* newVert = new Vertex(this->cords);

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
