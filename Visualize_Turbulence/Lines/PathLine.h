#ifndef PATHLINE_H
#define PATHLINE_H

#include <vector>
#include "Geometry/Vertex.h"

class Mesh;
// the pathline traced from a single seed
class PathLine{
public:
    // first vertex saved in Vertex is the seed
    // and the first vertex is the pathline at t=0
    vector<Vertex*> verts;

    PathLine();
    PathLine(vector<Vertex*> vs);
    ~PathLine();

    inline unsigned long num_verts();
};

void place_seeds(vector<PathLine*>& pls);
void build_pathlines_from_seeds(Mesh* mesh);
Vector3d trace_one_time_step(const Vector3d& start_cords, const Vector3d& vel);

inline unsigned long PathLine::num_verts()
{
    return this->verts.size();
}

#endif // PATHLINE_H
