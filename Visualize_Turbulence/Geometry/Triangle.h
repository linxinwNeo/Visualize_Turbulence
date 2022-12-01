#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include "Others/Predefined.h"
#include "Others/Vector3d.h"

// forward class declarations
class Vertex;
class Tet;
class Edge;

using namespace std;

class Triangle {
public:
    // member variables
    unsigned long idx;
    vector<Vertex*> verts;      // a face can have 3/4/5... number of vertices, depending on the element type
    vector<Edge*> edges;        // edges that made of this face, can be more than 3.
    vector<Tet*> tets;          // tets that uses this face, should be one/two only.
    Vector3d normal;

    // member functions
    Triangle();
    Triangle(Vertex*, Vertex*, Vertex*);
    ~Triangle();

    inline UL num_verts() const;
    inline UL num_edges() const;
    inline unsigned long num_tets() const;

    inline void add_vert(Vertex*);
    inline void add_edge(Edge*);
    inline void add_tets(Tet*);
    bool has_vert(const Vertex*) const;
    bool has_edge(const Vertex*, const Vertex*) const;
    Vertex* not_has_vert(const vector<Vertex*>) const;
};


inline UL Triangle::num_verts() const
{
    return this->verts.size();
}


inline UL Triangle::num_edges() const
{
    return this->edges.size();
}


inline UL Triangle::num_tets() const
{
    return this->tets.size();
}


inline void Triangle::add_vert(Vertex* v){
    this->verts.push_back(v);
}


inline void Triangle::add_edge(Edge* e){
    this->edges.push_back(e);
}


inline void Triangle::add_tets(Tet* tet){
    this->tets.push_back(tet);
}


#endif // TRIANGLE_H
