#include "Geometry/Mesh.h"
#include "Others/Utilities.h"
#include "Analysis/FixedPtDetect.h"
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <iostream>
#include <queue>
#include <stdlib.h>
#include <time.h>


unordered_map< double, vector<Singularity*> > Mesh::detect_sings()
{
    qDebug() << "start detecting singularities";
    unordered_map< double, vector<Singularity*> > sings_for_all_t;

    double cur_time = 0;
    while( cur_time < this->num_time_steps - 1. ){
        // create candidate tet list at cur_time
        // note that each tet is deep-copied from the original tet in the mesh
        // we copied vertices and edges, remeber to free them when done
        vector<Tet*> candidates = this->build_candidate_tets(cur_time);
        vector<Singularity*>temp;
        sings_for_all_t[cur_time] = temp;
        // for each candidate tet, we try to find critical point inside it.
        for(UI i = 0; i < candidates.size(); i++){
            Tet* tet = candidates[i];
            Vector3d* fixed_pt_cords = nullptr;
            // try to find the critical point using tetrahedron subdivision method
            find_fixed_pt_location_TetSubd(tet, cur_time, &fixed_pt_cords);

            // check if we find the fixed point or not
            if(fixed_pt_cords != nullptr){
                // calculate the jacobian matrix on the fixed point location
                Singularity* sing = new Singularity();
                sing->cords = fixed_pt_cords;
                sing->Jacobian = tet->calc_Jacobian(fixed_pt_cords, cur_time);
                sing->classify_this(); // classify the type of the singularity
                sing->in_which_tet = this->tets[tet->idx]; // record the which tet contains this singularity
                sings_for_all_t[cur_time].push_back( sing ); // save the singularity in a vector
                delete fixed_pt_cords;
            }

            Utility::clear_mem(tet->verts);
            Utility::clear_mem(tet->edges);
            Utility::clear_mem(tet->tris);
            delete tet;
            candidates[i] = nullptr;
        }
        candidates.clear();

        cur_time += time_step_size;
    }

    qDebug() << "finish detecting singularities";
    return sings_for_all_t;
}


bool Mesh::is_candidate_tet(Tet* tet, const double time) const
{
    bool pos_x = false, neg_x = false;
    bool pos_y = false, neg_y = false;
    bool pos_z = false, neg_z = false;

    for(const Vertex* vert : tet->verts){
        Vector3d* vel = vert->vels.at(time);
        double x = vel->x();
        double y = vel->y();
        double z = vel->z();

        if(x > 0) pos_x = true;
        if(x < 0) neg_x = true;

        if(y > 0) pos_y = true;
        if(y < 0) neg_y = true;

        if(z > 0) pos_z = true;
        if(z < 0) neg_z = true;
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
        // ignore boudnary tetrahedrons
        if(tet->has_boundary_tri()) continue;

        // check if it is a candidate tet
        if( is_candidate_tet(tet, time) ){
            tet->marked = true;
            Vertex* v1 = tet->get_vert_at(tet->verts[0]->cords, time, ws, true, true);
            Vertex* v2 = tet->get_vert_at(tet->verts[1]->cords, time, ws, true, true);
            Vertex* v3 = tet->get_vert_at(tet->verts[2]->cords, time, ws, true, true);
            Vertex* v4 = tet->get_vert_at(tet->verts[3]->cords, time, ws, true, true);
            Tet* new_tet = new Tet(v1, v2, v3, v4);
            new_tet->idx = tet->idx;
            new_tet->make_edges();
            new_tet->make_triangles();
            candidates.push_back(new_tet);
        }
    }
    qDebug() << "num of candidates" << candidates.size();
    return candidates;
}


char Mesh::Positive( const Vector3d* v1, const Vector3d* v2, const Vector3d* v3, const Vector3d* v4, const double time ) const {
    Eigen::Matrix4d m;
    m << v1->x(), v1->y(), v1->z(), 1,
             v2->x(), v2->y(), v2->z(), 1,
             v3->x(), v3->y(), v3->z(), 1,
             v4->x(), v4->y(), v4->z(), 1;

    const double det = m.determinant();

    if(det < 0) return -1;
    else if(det > 0) return 1;
    else {
        //        Utility::throwErrorMessage("determinant is 0");
        return 0;
    }
}

// http://www.sci.utah.edu/publications/Bha2014b/Bhatia_TopoInVis2014.pdf
bool Mesh::has_fixedPt_Robust( const Tet* tet, const double time ) const {
    Vertex* vert1 = tet->verts[0];
    Vertex* vert2 = tet->verts[1];
    Vertex* vert3 = tet->verts[2];
    Vertex* vert4 = tet->verts[3];

    const Vector3d* v1 = vert1->vels[time];
    const Vector3d* v2 = vert2->vels[time];
    const Vector3d* v3 = vert3->vels[time];
    const Vector3d* v4 = vert4->vels[time];
    vector<const Vector3d*> vs = {v1, v2, v3, v4};

    const Vector3d* zero = new Vector3d();

    char s = this->Positive(vs[0], vs[1], vs[2], vs[3], time);
    for(char i=0; i<4; i++){
        vs[i] = zero;
        char s_i = this->Positive(vs[0], vs[1], vs[2], vs[3], time);
        vs[i] = tet->verts[i]->vels[time];
        if(s != s_i) return false;
    }

    return true;
}


// for each time step, find tets with fixed pts
void Mesh::find_tets_with_fixedPts()
{
    double cur_time = 0.;

    while( cur_time < this->num_time_steps - 1. ){
        vector<Tet*> tets_with_fixed_pt;
        tets_with_fixed_pt.reserve(100);
        for(Tet* tet : this->tets){
            if(tet->has_boundary_tri()) continue;
           if( this->has_fixedPt_Robust(tet, cur_time) ){
               // TODO: should we do a simpler candidate test first?
               tets_with_fixed_pt.push_back(tet);
           }
        }
        this->tet_with_fixed_pt_for_all_t[cur_time] = tets_with_fixed_pt;
        cur_time += time_step_size;
    }
    return;
}


/* find one fixed pt in a given tet, assume only one can exist in a tet.
 * The idea is to subdivide the tetrahedron recursively until we found the a critical point.
 * Steps:
 * 1. make a queue to store tetrahedrons in order (breadth first)
 * 2. for each tet, we check (1) if it is valid tet, (2) if one of its vertices is a critical pt.
 * 3. we subdivide the tet if it is valid tet and none vertex is a critical pt.
 * 4. place new tets into queue can continue to next loop.
 * 5. if find a vertex, return the critical pt as fixed_pt.
 * We use a iterative method rather than using recursive method since iterative has a silghtly better performance.
 *
 * parameters:
 * Tet * tet_to_be_checked: the tet that need to be checked if there is a critical pt inside
 * const double time: we need the velocity defined at some time to check if there is a critical point.
 * Vector3d** fixed_pt: If we found a critical point, return the position of the fixed point
*/
UI Mesh::find_fixed_pt_location_TetSubd( const Tet *tet_to_be_checked, const double time, Vector3d** fixed_pt ) const
{
    // a queue is FIFO, so it is the breadth-first search
    std::queue<Tet*> candidate_tets;
    // verts and edges created when we subdivide, both need to be cleared before return
    vector<Vertex*> temp_verts; vector<Edge*> temp_edges; vector<Triangle*> temp_tris;

    // copy the tet_to_be_checked
    // only copies the vertices and velocity at time
    Tet* tet_cp = tet_to_be_checked->clone(time);
    temp_verts.push_back(tet_cp->verts[0]);
    temp_verts.push_back(tet_cp->verts[1]);
    temp_verts.push_back(tet_cp->verts[2]);
    temp_verts.push_back(tet_cp->verts[3]);

    candidate_tets.push(tet_cp); // push the original tet into the queue to start the following while loop


    // calculating the max number of tetrahedrons that need to be checked for the incoming tet
    const UI max_times = pow(8, max_num_recursion);
    // records how many tets have been checked
    UI n = 0;


    Tet* tet = nullptr;
    // main loop
    while( candidate_tets.size() != 0 ){
        // check if max_times is reached
        if( n >= max_times ) break;

        // pop the first tet from the queue
        tet = candidate_tets.front();
        candidate_tets.pop();
        n++;

        // check if one of vertices of the tet is a singularity
        for(unsigned char i = 0; i < 4; i++){
            const Vertex* v = tet->verts[i];
            // if we found, copy the vertex, save it and return
            if( length( *(v->vels.at(time)) ) < zero_threshold ){
                *fixed_pt = new Vector3d(v->cords);
                goto LL; // exit the function
            }
        }

        // if we didn't find a fixed pt on vertices, we subdivide
        vector<Tet*> new_candidates;
        // subdivision will create 8 smaller tetrahedrons and those tetrahedrons also create vertices and edges
        tet->subdivide(time, temp_verts, temp_edges, temp_tris, new_candidates);

        if(new_candidates.size() != 8) Utility::throwErrorMessage( "Mesh::find_fixed_pt_location_TetSubd: Error! The size of new_candidates is not 8!");

        for(Tet* new_tet : new_candidates){
            // check if the new tet is a candidate
            if( is_candidate_tet(new_tet, time) ){
                candidate_tets.push(new_tet);
                continue;
            }
            delete new_tet;
            new_tet = nullptr;
        }

        // get ready for next iteration
        delete tet;
        tet = nullptr;
    }

    // exiting the function, clear the memory
    LL:
        qDebug() << "size of candidates" << candidate_tets.size();
        Utility::clear_mem(temp_verts);
        Utility::clear_mem(temp_edges);
        Utility::clear_mem(temp_tris);
        Utility::clear_mem(candidate_tets);
        if(tet != nullptr) delete tet;

    return n;
}

