#ifndef TET_H
#define TET_H

#include "Others/Point.h"
#include "Others/Vector3d.h"
#include <vector>

// forward class declarations
class Vertex;
class Edge;
class Triangle;

using namespace std;

class Tet {
public:
    // member variables
    unsigned long idx;
    vector<Vertex*> verts;  // exact 4 verts that consists of this tetrahedron
    vector<Edge*> edges;    // exact 4 edges that consists of this tetrahedron
    vector<Triangle*> tris;    // exact 4 triangles that consists of this tetrahedron
    vector<Tet*> tets;      // neighbor tetrahedrons that shares faces with this, max of 4 tets

    // member functions
    Tet();
    Tet(Vertex*, Vertex*, Vertex*, Vertex*);
    Tet(Triangle* tri, Vertex *v4);
    ~Tet();

    inline unsigned long num_verts() const;
    inline unsigned long num_edges() const;
    inline unsigned long num_tris() const;
    inline unsigned long num_tets() const;

    inline void add_vert(Vertex*);
    inline void add_edge(Edge*);
    inline void add_triangle(Triangle*);
    inline void add_tet(Tet*);
    bool has_vert(const Vertex*) const;
    bool has_verts(const Vertex*, const Vertex*, const Vertex*) const;
    bool has_triangle(const Vertex*, const Vertex*, const Vertex*) const;
    bool has_edge(const Vertex*, const Vertex*) const;
    // https://stackoverflow.com/questions/25179693/how-to-check-whether-the-point-is-in-the-tetrahedron-or-not
    bool is_same_side(const Vector3d&, const Vector3d&, const Vector3d&, const Vector3d&, const Vector3d&) const;
    bool is_pt_in(const Point*) const;

    Vertex* get_vert_at(const Point*, const double time);
    double volume() const;
    Point centroid() const;
};


inline unsigned long Tet::num_verts() const
{
    return this->verts.size();
}


inline unsigned long Tet::num_edges() const
{
    return this->edges.size();
}


inline unsigned long Tet::num_tris() const
{
    return this->tris.size();
}


inline unsigned long Tet::num_tets() const
{
    return this->tets.size();
}


inline void Tet::add_vert(Vertex* v){
    this->verts.push_back(v);
}


inline void Tet::add_edge(Edge* e){
    this->edges.push_back(e);
}


inline void Tet::add_triangle(Triangle* f){
    this->tris.push_back(f);
}


inline void Tet::add_tet(Tet* tet){
    this->tets.push_back(tet);
}


#endif // TET_H
