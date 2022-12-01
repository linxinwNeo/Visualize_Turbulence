#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QString>
#include <QDebug>
#include "Geometry/Vertex.h"
#include "Geometry/Edge.h"
#include "Geometry/Triangle.h"
#include "Geometry/Tet.h"
#include "Others/Vector3d.h"


using namespace std;


class Mesh {
public:
    // member variables
    vector<Vertex*> verts;
    vector<Edge*> edges;
    vector<Triangle*> tris;
    vector<Tet*> tets;
    Vector3d rot_center;
    double radius;
    unsigned int num_time_steps;

    // member functions
    Mesh();
    ~Mesh();

    inline unsigned long num_verts() const;
    inline unsigned long num_edges() const;
    inline unsigned long num_tris() const;
    inline unsigned long num_tets() const;

    inline void add_vert(Vertex*);
    inline void add_edge(Edge*);
    inline void add_triangle(Triangle*);
    inline void add_tet(Tet*);

    void calc_Bounding_Sphere();
    void build_triangles();
    void build_edges( );
    void assign_triangle(Tet*, Tet*, Vertex*, Vertex*, Vertex*); // without edges
    void assign_edge(Vertex*, Vertex*);
    void max_vor_mag(const unsigned int, double&, double&) const;
};


inline unsigned long Mesh::num_verts() const
{
    return this->verts.size();
}


inline unsigned long Mesh::num_edges() const
{
    return this->edges.size();
}


inline unsigned long Mesh::num_tris() const
{
    return this->tris.size();
}


inline unsigned long Mesh::num_tets() const
{
    return this->tets.size();
}


inline void Mesh::add_vert(Vertex* v){
    v->idx = this->num_verts();
    this->verts.push_back(v);
}


inline void Mesh::add_edge(Edge* e){
    e->idx = this->num_edges();
    this->edges.push_back(e);
}


inline void Mesh::add_triangle(Triangle* tri){
    tri->idx = this->num_tris();
    this->tris.push_back(tri);
}


inline void Mesh::add_tet(Tet* tet){
    tet->idx = this->num_tets();
    this->tets.push_back(tet);
}


#endif // MESH_H
