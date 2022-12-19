#include "Geometry/Mesh.h"
#include "Others/Utilities.h"

UI depth = 0;
void capture_critical_pts(Mesh* mesh){
    if(mesh == NULL) return;
    mesh->detect_fixed_pts();
}


void capture_critical_pts(vector<Mesh*> meshes){
    for(UI i = 0; i < meshes.size(); i++){
        Mesh* mesh = meshes[i];
        qDebug() << "Capturing critical pts for mesh" << i+1;
        capture_critical_pts(mesh);
    }
}


void Mesh::detect_fixed_pts()
{
    double cur_time = 0;
    while( cur_time < this->num_time_steps - 1. ){
        vector<Tet*> candidates = this->build_candidate_tets(cur_time);
        qDebug() << "time" << cur_time << "num of candidates" << candidates.size();
        for(UI i = 0; i < candidates.size(); i++){
            qDebug() << i;
            Tet* tet = candidates[i];
            vector<Vertex*> fixed_pts;
            depth = 0;

            find_fixed_pt_location(tet, cur_time, 0, fixed_pts);
            if(fixed_pts.size() != 0 )
                qDebug() << "num of fixed pts"  << fixed_pts.size();

            // else: classify the type of the critical point by eigen decomposition
            if(depth != max_num_recursion+1) qDebug() << depth;

            Utility::clear_mem(tet->verts);
            Utility::clear_mem(tet->edges);
            delete tet;

            for(auto& vert : fixed_pts){
                delete vert;
            }
            fixed_pts.clear();
        }

        cur_time += time_step_size;
    }
}


bool Mesh::is_candidate_tet(Tet* tet, const double time) const
{
    bool pos_x = false, neg_x = false;
    bool pos_y = false, neg_y = false;
    bool pos_z = false, neg_z = false;

    for(const Vertex* vert : tet->verts){
        Vector3d* vel = vert->vels.at(time);
        if(vel->x() >= 0) pos_x = true;
        else neg_x = true;

        if(vel->y() >= 0) pos_y = true;
        else neg_y = true;

        if(vel->z() >= 0) pos_z = true;
        else neg_z = true;
    }

    return pos_x && neg_x && pos_y && neg_y && pos_z && neg_z;
}


// build an array that contains tets that may contains the feature point/singularity
// for each edge, detect if the component of the velocity vector changes its sign
// if all components changes sign in a tet, then we place it in the list
// candidate tets are copied from original tet
vector<Tet*> Mesh::build_candidate_tets( const double time ) const
{
    vector<Tet*> candidates;
    double ws[4];
    for(Tet* tet : this->tets){
        if( is_candidate_tet(tet, time) ){
            Vertex* v1 = tet->get_vert_at(tet->verts[0]->cords, time, ws, true, true);
            Vertex* v2 = tet->get_vert_at(tet->verts[1]->cords, time, ws, true, true);
            Vertex* v3 = tet->get_vert_at(tet->verts[2]->cords, time, ws, true, true);
            Vertex* v4 = tet->get_vert_at(tet->verts[3]->cords, time, ws, true, true);
            Tet* new_tet = new Tet(v1, v2, v3, v4);
            new_tet->make_edges();
            candidates.push_back(new_tet);
        }
    }

    return candidates;
}


// check if vertex to see if anyone has velocity of 0
// if not, we recursively subdivide each tet into 4 new tetrahedrons
// the new node is the centroid.
void Mesh::find_fixed_pt_location(
        Tet *tet, const double time, UI cur_depth,
        vector<Vertex*>& fixed_pts) const
{
    if(cur_depth > depth) depth = cur_depth;
    // base case
    if(cur_depth > max_num_recursion) return;

    // check if any of the vertex of the tetrahedron is the critical point
    for(unsigned int i = 0; i < 4; i++){
        Vertex* v = tet->verts[i];
        if( length( v->vels[time] ) <= zero_threshold )
        {
             // copy vertex to be returned
            Vertex* new_vert = new Vertex(v->cords);
            new_vert->vels[time] = new_vert->vels[time];
            fixed_pts.push_back(new_vert);
            return;
        }
    }

    // if 4 vertices don't match, we need to subdivide the current tetrahedron into smaller ones (8)
    vector<Vertex*> temp_verts; vector<Edge*> temp_edges; vector<Tet*> temp_tets;
    tet->subdivide(time, temp_verts, temp_edges, temp_tets);
    for(Tet* tet : temp_tets){
        if(!is_candidate_tet(tet, time)) continue;

        find_fixed_pt_location(tet, time, cur_depth+1, fixed_pts);
    }

    Utility::clear_mem(temp_verts);
    Utility::clear_mem(temp_edges);
    Utility::clear_mem(temp_tets);
}
