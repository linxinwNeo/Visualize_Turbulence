#include "Geometry/Mesh.h"
#include "Others/Utilities.h"

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
            Tet* tet = candidates[i];
            vector<Vertex*> temp_verts; vector<Edge*> temp_edges;
            vector<Vertex*> fixed_pts;
            find_fixed_pt_location(tet, cur_time, 0, temp_verts, temp_edges, fixed_pts);
            if(fixed_pts.size() != 0 )
                qDebug() << "num of fixed pts"  << fixed_pts.size();
            // clear memory
            clear_memory(temp_verts); clear_memory(temp_edges);

            // else: classify the type of the critical point by eigen decomposition

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

    for(Tet* tet : this->tets){
        if( is_candidate_tet(tet, time) ){
            Tet* new_tet = new Tet(tet->verts[0], tet->verts[1], tet->verts[2], tet->verts[3]);
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
        vector<Vertex*> temp_verts, vector<Edge*> temp_edges,
        vector<Vertex*> fixed_pts) const
{
    // base case
    if(cur_depth > max_num_recursion) return;

    // check if vertex is the critical point
    for(unsigned int i = 0; i < 4; i++){
        Vertex* v = tet->verts[i];
        if( length( v->vels.at(time) ) <= zero_threshold )
        {
             // copy vertex to be returned
            Vertex* new_vert = new Vertex(v->cords);
            new_vert->vels[time] = new_vert->vels[time];
            fixed_pts.push_back(new_vert);
            return;
        }
    }

    // if 4 vertices don't match, we need to constuct new tets
    vector<Tet*> new_tets = tet->make_4_tets(time);

    for(Tet* new_tet : new_tets){
        // insert edges and verts to temp vectors
        for(Edge* e : new_tet->edges){
            temp_edges.push_back(e);
        }
        for(Vertex* v : new_tet->verts){
            temp_verts.push_back(v);
        }

        // check if this new tet is a candidate tet
        if( is_candidate_tet(new_tet, time) ){
            // if yes, recursively call find_fixed_pt_location
            find_fixed_pt_location(new_tet, time, cur_depth+1, temp_verts, temp_edges, fixed_pts);
            delete new_tet;
        }
        else
        {
            // if not, delete
            delete new_tet;
        }
    }

    // if we reach here, this means this tet does not contain critical point.
    return;
}
