#ifndef TET_H
#define TET_H

#include <unordered_map>
#include <vector>
#include "Others/Vector3d.h"
#include "Eigen/Dense"

// forward class declarations
class Vertex;
class Edge;
class Triangle;

using namespace std;

class Tet {
public:
    // member variables
    unsigned long idx;
    unordered_map<double, unsigned char> marching_idices;
    vector<Vertex*> verts;  // exact 4 verts that consists of this tetrahedron
    vector<Edge*> edges;    // exact 4 edges that consists of this tetrahedron
    vector<Triangle*> tris;    // exact 4 triangles that consists of this tetrahedron
    vector<Tet*> tets;      // neighbor tetrahedrons that shares faces with this, max of 4 tets

    bool marked;
    Vector3d center;

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
    bool has_boundary_tri() const;


    Vertex* get_vert_at(const Vector3d& v, const double time, double ws[4], bool cal_ws = true, bool add_this_tet = true);
    double volume() const;
    Vector3d centroid() const;

    Tet* clone(const double time) const;

    Vector3d actual_normal_of( unsigned char tri_idx ) const;
    Vertex* missing_vertex(Vertex* v1, Vertex*v2, Vertex* v3) const;
    bool is_pt_inside(const Vector3d& P, const bool calc_ws, double ws[4]) const;
    void bary_cords(double ws[4], const Vector3d& P) const;
    double ScTP(const Vector3d &a, const Vector3d &b, const Vector3d &c) const;
    void bary_tet(const Vector3d & p, double ds[4]) const;
    bool is_pt_in2(const Vector3d& p, double ds[4]) const;

    void calc_marching_indices(unsigned int num_time_steps);

    vector<Triangle*> create_isosurface_tris(const double& time);
    Triangle* create_isosurface_tris_case1234( const Vertex* v, const double time );
    vector<Triangle*> create_isosurface_tris_case567( const Vertex* v1, const Vertex* v2, const double time );
    void make_edges();
    void make_triangles();
    void subdivide(const double time, vector<Vertex*>& new_verts, vector<Edge*>& new_edges, vector<Triangle*>& temp_tris, vector<Tet*>& new_tets);
    Eigen::Matrix3d calc_Jacobian(const Vector3d& cords, const double time);
};

bool is_same_side(const Vector3d&, const Vector3d&, const Vector3d&, const Vector3d&, const Vector3d&);

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
