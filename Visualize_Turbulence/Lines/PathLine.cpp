#include "Lines/PathLine.h"
#include "Others/Predefined.h"
#include "Others/Utilities.h"

PathLine::PathLine()
{

}

PathLine::PathLine(vector<Vertex*> vs)
{
    this->verts.reserve(vs.size());

    for(auto& v : vs){
        this->verts.push_back(v);
    }
}

PathLine::~PathLine()
{
    for(UL i = 0; i < this->num_verts(); i++){
        if(this->verts[i] != NULL) delete this->verts[i];
    }

    this->verts.clear();
}


// now every pathline has a seed point, we want to calculate their path individually
void build_pathlines_from_seeds(Mesh* mesh){
    // for each pathline, we calculate their trajectories

}


// tracing the start_cords one time step
// vel is the velocity vector
// return the resultant cords
Vector3d trace_one_time_step(const Vector3d& start_cords, const Vector3d& vel)
{
    return start_cords + vel*time_step_size;
}
