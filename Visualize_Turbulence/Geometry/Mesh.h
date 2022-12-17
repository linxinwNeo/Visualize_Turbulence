#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QString>
#include <QDebug>

#include "Geometry/Vertex.h"
#include "Geometry/Edge.h"
#include "Geometry/Triangle.h"
#include "Geometry/Tet.h"
#include "Lines/StreamLine.h"
#include "Others/Predefined.h"
#include "Others/Vector3d.h"
#include "Surfaces/Isosurface.h"

using namespace std;

class Mesh {
public:
    // member variables
    vector<Vertex*> verts;
    vector<Edge*> edges;
    vector<Triangle*> tris;
    vector<Tet*> tets;

    vector<Triangle*> boundary_tris;

    Vector3d rot_center;
    //double radius;
    unsigned int num_time_steps;

    unordered_map<double, pair<double,double>> min_max_at_verts_for_all_t;

    // we calculate streamlines for each original time steps, so [0] means the strealines for the first time step
    unordered_map< double, vector<StreamLine*> > streamlines_for_all_t;
    unordered_map< double, Isosurface*> isosurfaces_for_all_t;

    unordered_map<double, vector<Vertex*>> singularities_for_all_t;

    // member functions
    Mesh();
    ~Mesh();

    inline unsigned long num_verts() const;
    inline unsigned long num_edges() const;
    inline unsigned long num_tris() const;
    inline unsigned long num_tets() const;
    inline unsigned long num_boundary_tris() const;

    inline void add_vert(Vertex*);
    inline void add_edge(Edge*);
    inline void add_triangle(Triangle*);
    inline void add_tet(Tet*);
    inline void add_vor_min_max_at_verts_for_all_t( const double time, const pair<double, double> min_max_pair );

    void calc_Bounding_Sphere();
    void build_triangles();
    void build_edges( );
    void build_tetNeighbors();
    void assign_triangle(Tet*, Tet*, Vertex*, Vertex*, Vertex*); // without edges
    void assign_edge(Vertex*, Vertex*);
    void max_vor_mag(const double t, double& min, double& max) const;
    void max_vel_mag(const double t, double& min, double& max) const;
    void calc_normal_for_all_tris();
    void calc_vor_min_max_at_verts_for_all_t();
    void calc_center_for_all_tet();

    // numerical procedures
    void interpolate_vertices();
    Tet* inWhichTet(const Vector3d& target_pt, Tet* prev_tet, double ds[4]) const;

    // singularity detection
    void detect_fixed_pts();
    bool is_candidate_tet(Tet* tet, const double time) const;
    vector<Tet*> build_candidate_tets( const double time ) const;
    void find_fixed_pt_location( Tet *tet, const double time, UI cur_depth,
                                    vector<Vertex*> temp_verts, vector<Edge*> temp_edges,
                                    vector<Vertex*> fixed_pts) const;
};
void capture_critical_pts(Mesh* mesh);
void capture_critical_pts(vector<Mesh*> meshes);

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


inline unsigned long Mesh::num_boundary_tris() const
{
    return this->boundary_tris.size();
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


inline void Mesh::add_vor_min_max_at_verts_for_all_t(const double time, const pair<double, double> min_max_pair)
{
    this->min_max_at_verts_for_all_t[time] = min_max_pair;
}


#endif // MESH_H
