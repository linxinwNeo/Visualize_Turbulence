#ifndef EDGE_H
#define EDGE_H

#include "Others/Vector3d.h"
#include <vector>

// forward class declarations
class Vertex;
class Tet;
class Triangle;

using namespace std;

class Edge {
public:
    // member variables
    unsigned long  idx;
    vector<Vertex*> verts;    // an edge has two vertices
    vector<Triangle*> tris;    // faces that contain this edge
    vector<Tet*> tets;      // tetrahedrons that contain this edge

    // member functions
    inline Edge();
    inline Edge(Vertex*, Vertex*);
    inline ~Edge();

    inline unsigned long num_verts() const;
    inline unsigned long num_tris() const;
    inline unsigned long num_tets() const;

    inline void add_vert(Vertex*);
    inline void add_triangle(Triangle*);
    inline void add_tet(Tet*);

    bool is_same(const Edge*) const;
    inline bool has_vert(const Vertex* v) const;
    bool is_vel_change_sign_in_x( const double time ) const;
    bool is_vel_change_sign_in_y( const double time ) const;
    bool is_vel_change_sign_in_z( const double time ) const;

    Vertex* linear_interpolate_basedOn_vorMag( const double& time, const double& target_val );
};


// constructor
inline Edge::Edge()
{
    this->idx = 0;
    // reserve memories
    this->verts.reserve(2); // an edge should only have 2 verts
}


inline Edge::Edge(Vertex* v1, Vertex* v2){
    this->add_vert(v1);
    this->add_vert(v2);
}


// destructor
inline Edge::~Edge()
{
    this->verts.clear();
    this->tris.clear();
    this->tets.clear();
}


inline unsigned long Edge::num_verts() const
{
    return this->verts.size();
}


inline unsigned long Edge::num_tris() const
{
    return this->tris.size();
}


inline unsigned long Edge::num_tets() const
{
    return this->tets.size();
}


inline void Edge::add_vert(Vertex* v){
    this->verts.push_back(v);
}


inline void Edge::add_triangle(Triangle* f){
    this->tris.push_back(f);
}


inline void Edge::add_tet(Tet* tet){
    this->tets.push_back(tet);
}

inline bool Edge::has_vert(const Vertex *v) const
{
    for(Vertex* vert : this->verts){
        if(vert == v) return true;
    }
    return false;
}


#endif // EDGE_H
