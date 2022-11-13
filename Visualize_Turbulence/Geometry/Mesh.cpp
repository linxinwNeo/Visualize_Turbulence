#include "Mesh.h"
#include <QDebug>

Mesh::Mesh()
{
    // reserve vector memories to save time
    this->edges.reserve(10000);
    this->faces.reserve(10000);
}


Mesh::~Mesh()
{
    // free memories of each vector
    unsigned long i;
    for( i=0; i<this->num_verts(); i++ ){
        if( this->verts[i] != NULL ){
            delete this->verts[i];
            this->verts[i] = NULL;
        }
    }


    for( i=0; i<this->num_edges(); i++ ){
        if( this->edges[i] != NULL ){
            delete this->edges[i];
            this->edges[i] = NULL;
        }
    }


    for( i=0; i<this->num_faces(); i++ ){
        if( this->faces[i] != NULL ){
            delete this->faces[i];
            this->faces[i] = NULL;
        }
    }


    for( i=0; i<this->num_tets(); i++ ){
        if( this->tets[i] != NULL ){
            delete this->tets[i];
            this->tets[i] = NULL;
        }

    }
    this->verts.clear();
    this->edges.clear();
    this->faces.clear();
    this->tets.clear();
}


unsigned long Mesh::num_verts()
{
    return this->verts.size();
}


unsigned long Mesh::num_edges()
{
    return this->edges.size();
}


unsigned long Mesh::num_faces()
{
    return this->faces.size();
}


unsigned long Mesh::num_tets()
{
    return this->tets.size();
}


void Mesh::add_vert(Vertex* v){
    v->idx = this->num_verts();
    this->verts.push_back(v);
}


void Mesh::add_edge(Edge* e){
    e->idx = this->num_edges();
    this->edges.push_back(e);
}


void Mesh::add_face(Face* f){
    f->idx = this->num_faces();
    this->faces.push_back(f);
}


void Mesh::add_tet(Tet* tet){
    tet->idx = this->num_tets();
    this->tets.push_back(tet);
}
