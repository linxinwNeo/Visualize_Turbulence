#include "Geometry/Mesh.h"
#include "Others/Utilities.h"
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <iostream>
#include <queue>
#include "Analysis/singularity.h"


void capture_critical_pts(Mesh* mesh){
    if(mesh == NULL) return;
    mesh->detect_fixed_pts();
}


void capture_critical_pts(vector<Mesh*> meshes){
    for(UI i = 0; i < meshes.size(); i++){
        Mesh* mesh = meshes[i];
        qDebug() << "Capturing critical pts for mesh" << i+1;
        mesh->interpolate_vertices();
        capture_critical_pts(mesh);
    }
}


void Mesh::detect_fixed_pts()
{
    double cur_time = 0;
    while( cur_time < this->num_time_steps - 1. ){
        vector<Tet*> candidates = this->build_candidate_tets(cur_time);
        vector<Singularity*>temp;
        this->singularities_for_all_t[cur_time] = temp;
        qDebug() << "time" << cur_time << "num of candidates" << candidates.size();
        for(UI i = 0; i < candidates.size(); i++){
            Tet* tet = candidates[i];
            vector<Vertex*> fixed_pts;

            UI d = find_fixed_pt_location(tet, cur_time, fixed_pts);

            for(Vertex* fixed_pt : fixed_pts){
                // calculate the jacobian matrix
                Singularity* sing = new Singularity();
                sing->Jacobian = tet->calc_Jacobian(fixed_pt, cur_time);
                sing->classify_this();
                sing->cords = fixed_pt->cords;
                sing->in_which_tet = tets[tet->idx];
                qDebug() << sing->get_type();
                this->singularities_for_all_t[cur_time].push_back( sing );
            }

            Utility::clear_mem(tet->verts);
            Utility::clear_mem(tet->edges);
            Utility::clear_mem(fixed_pts);
            delete tet;
        }

        if(this->singularities_for_all_t[cur_time].size() != 0)
            qDebug() << "num of fixed pts for time" << cur_time << "is" << this->singularities_for_all_t[cur_time].size();
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
        if(vel->x() <= 0) neg_x = true;

        if(vel->y() >= 0) pos_y = true;
        if(vel->y() <= 0) neg_y = true;

        if(vel->z() >= 0) pos_z = true;
        if(vel->z() <= 0) neg_z = true;
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
            tet->marked = true;
            Vertex* v1 = tet->get_vert_at(tet->verts[0]->cords, time, ws, true, true);
            Vertex* v2 = tet->get_vert_at(tet->verts[1]->cords, time, ws, true, true);
            Vertex* v3 = tet->get_vert_at(tet->verts[2]->cords, time, ws, true, true);
            Vertex* v4 = tet->get_vert_at(tet->verts[3]->cords, time, ws, true, true);
            Tet* new_tet = new Tet(v1, v2, v3, v4);
            new_tet->idx = tet->idx;
            new_tet->make_edges();
            candidates.push_back(new_tet);
        }
    }

    return candidates;
}


// check if vertex to see if anyone has a velocity of 0
// if not, we recursively subdivide each tet into 8 new tetrahedrons
// stop once we found a fixed point
UI Mesh::find_fixed_pt_location(
        Tet *tet, const double time,
        vector<Vertex*>& fixed_pts) const
{
    UI max_times = pow(8, max_num_recursion);
    UI cur_times = 0;
    std::queue<Tet*> candidates;
    candidates.push(tet);
    vector<Vertex*> temp_verts; vector<Edge*> temp_edges;

    while(candidates.size() != 0){
        if(cur_times >= max_times) break;

        Tet* tet = candidates.front();
        candidates.pop();
        if(!is_candidate_tet(tet, time)) continue;
        cur_times ++;

        // check if any of the vertex of the tetrahedron is the critical point
        for(unsigned int i = 0; i < 4; i++){
            Vertex* v = tet->verts[i];
            if( length(v->vels[time]) > zero_threshold ) continue;
             // copy vertex to be returned
            Vertex* new_vert = new Vertex(v->cords);
            new_vert->vels[time] = new Vector3d( v->vels[time] );
            fixed_pts.push_back(new_vert);

            Utility::clear_mem(temp_verts);
            Utility::clear_mem(temp_edges);
            return cur_times;
        }

        // if 4 vertices don't match, we need to subdivide the current tetrahedron into smaller ones (8)
        vector<Tet*> temp_tets;
        tet->subdivide(time, temp_verts, temp_edges, temp_tets);
        for(Tet* tet2 : temp_tets){
            if(!is_candidate_tet(tet2, time)) continue;
            candidates.push(tet2);
        }
    }

    // ran out of recusion depth
    Utility::clear_mem(temp_verts);
    Utility::clear_mem(temp_edges);
    return cur_times;
}

