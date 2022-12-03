#include "Geometry/Triangle.h"
#include "Geometry/Vertex.h"
#include "Geometry/Edge.h"

// constructor
Triangle::Triangle()
{
    this->idx = 0;
    this->on_boundary = false;
    // reserve memories
    this->verts.reserve(3); // a face at least has 3 verts
    this->edges.reserve(3); // a face at least has 3 edges
    this->tets.reserve(2);   // a face can be shared by at most 2 tet
}

Triangle::Triangle(Vertex * v1, Vertex * v2, Vertex * v3)
{
    this->on_boundary = false;
    this->add_vert(v1);
    this->add_vert(v2);
    this->add_vert(v3);
}


// desctructor
Triangle::~Triangle()
{
    this->verts.clear();
    this->edges.clear();
    this->tets.clear();
}


bool Triangle::has_vert(const Vertex * v) const
{
    for( Vertex* vert : this->verts ){
        if(vert->idx == v->idx) return true;
    }
    return false;
}


bool Triangle::has_edge(const Vertex * v1, const Vertex * v2) const
{
    for(Edge* e : this->edges){
        bool has_v1 = false, has_v2 = false;
        Vertex* vv1 = e->verts[0];
        Vertex* vv2 = e->verts[1];
        if(vv1 == v1 || vv2 == v1){
            has_v1 = true;
        }
        if(vv1 == v2 || vv2 == v2){
            has_v2 = true;
        }
        if(has_v1 && has_v2) return true;
    }
    return false;
}


// return the first vertex that is not in the vector input
Vertex* Triangle::not_has_vert(const vector<Vertex *> vs) const
{
    for( Vertex* v : vs ){
        if(this->has_vert(v)) return v;
    }
    return NULL;
}
