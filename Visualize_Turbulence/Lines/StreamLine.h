#ifndef STREAMLINE_H
#define STREAMLINE_H

#include <vector>
#include "Geometry/Vertex.h"
#include "Others/Predefined.h"

using namespace std;

class Mesh;

class StreamLine
{
public:
    double time; // indicates which time this streamline is for
    Vertex* seed;
    vector<Vertex*> fw_verts; // first vertex is connected to the seed
    vector<Vertex*> bw_verts; // first vertex is connected to the seed

    StreamLine();
    StreamLine(vector<Vertex*> vs, const double& t);
    ~StreamLine();

    inline void set_seed(Vertex* seed);
    void add_fw_vert(Vertex* vert);
    void add_bw_vert(Vertex* vert);

    inline UL num_verts() const;
    inline UL num_fw_verts() const;
    inline UL num_bw_verts() const;
};

void tracing_streamlines();
void build_streamlines_from_seeds(Mesh* mesh);
Vector3d trace_one_dist_step(const Vector3d& start_cords, const Vector3d& vel);
void place_seeds(Mesh* mesh);
void place_critical_pts_as_seeds(Mesh* mesh);

inline void StreamLine::set_seed(Vertex *seed_vert)
{
    this->seed = seed_vert;
}


inline void StreamLine::add_fw_vert(Vertex *vert)
{
    this->fw_verts.push_back(vert);
}


inline void StreamLine::add_bw_vert(Vertex *vert)
{
    this->bw_verts.push_back(vert);
}


inline UL StreamLine::num_verts() const
{
    return this->fw_verts.size() + this->bw_verts.size();
}


inline UL StreamLine::num_fw_verts() const
{
    return this->fw_verts.size();
}


inline UL StreamLine::num_bw_verts() const
{
    return this->bw_verts.size();
}

#endif // STREAMLINE_H
