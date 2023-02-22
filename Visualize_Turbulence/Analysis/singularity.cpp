#include "Geometry/Mesh.h"
#include "Others/Utilities.h"
#include "Analysis/singularity.h"
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <iostream>
#include <queue>


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
        //qDebug() << "time" << cur_time << "num of candidates" << candidates.size();
        // for each candidate tet, we try to find critical point inside it.
        for(UI i = 0; i < candidates.size(); i++){
            Tet* tet = candidates[i];
            Vector3d* fixed_pt_cords  = nullptr;
            // try to find the critical point
            find_fixed_pt_location(tet, cur_time, &fixed_pt_cords);

            if(fixed_pt_cords != nullptr){
                // calculate the jacobian matrix
                Singularity* sing = new Singularity();
                sing->Jacobian = tet->calc_Jacobian(fixed_pt_cords, cur_time); // calculate the jacobian for this sing
    //            cout<< sing->Jacobian<<endl;
                sing->classify_this(); // classify the type of the singularity
                qDebug() << sing->get_type();
                sing->cords = fixed_pt_cords;
                qDebug() << "fixed_pt_cords: " << fixed_pt_cords->x() << fixed_pt_cords->y() << fixed_pt_cords->z();
                sing->in_which_tet = tets[tet->idx]; // record the which tet contains this singularity
                sings_for_all_t[cur_time].push_back( sing ); // save the singularity in a vector
                delete fixed_pt_cords;
            }

            Utility::clear_mem(tet->verts);
            Utility::clear_mem(tet->edges);
            Utility::clear_mem(tet->tris);
            delete tet;
        }

//        if(sings_for_all_t[cur_time].size() != 0) qDebug() << "num of singularity for time" << cur_time << "is" << sings_for_all_t[cur_time].size();
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
 * Vertex** fixed_pts: If we found a critical point, save that in the *fixed_pts and then return
*/
UI Mesh::find_fixed_pt_location( const Tet *tet_to_be_checked, const double time, Vector3d** fixed_pt ) const
{
    int num_pops = 0;
    // copy the tet_to_be_checked
    Tet* tet_cp = tet_to_be_checked->clone(time);

    // calculating the max number of tetrahedrons that need to be checked for the incoming tet
    const UI max_times = pow(8, max_num_recursion);
//     const UI max_times = 4;
    // records how many smaller tets have been checked
    UI count = 0;

    // a queue is FIFO, so it is breadth-first search
    std::queue<Tet*> candidates;

    candidates.push(tet_cp); // push the original tet into the queue to start the following while loop

    // verts and edges created when we subdivide, both need to be cleared before return
    vector<Vertex*> temp_verts; vector<Edge*> temp_edges; vector<Triangle*> temp_tris;

    Tet* tet = nullptr;
    // main loop
    while(candidates.size() != 0){
        // check if max_times is reached
        if( count >= max_times ) break;

        // pop the first tet from the queue
        tet = candidates.front();
        candidates.pop();
        num_pops++;

        // print out original tet's volume
//        qDebug() << "volume" << tet->volume();

        // check if one of vertices of tet is a singularity
        for(unsigned short i = 0; i < 4; i++){
            const Vertex* v = tet->verts[i];
            qDebug() << "vel is" << length( *(v->vels.at(time)));
            // if we found, copy the vertex, save it and return
            if( length( *(v->vels.at(time)) ) < zero_threshold ){
//            if(abs(v->vels.at(time)->x()) < zero_threshold && abs(v->vels.at(time)->y()) < zero_threshold && abs(v->vels.at(time)->z()) < zero_threshold ){
                qDebug() << "sing mag is" << length( *(v->vels.at(time)) );
                *fixed_pt = new Vector3d(v->cords);
                Utility::clear_mem(temp_verts);
                Utility::clear_mem(temp_edges);
                Utility::clear_mem(temp_tris);
                Utility::clear_mem(candidates);
                delete tet;
                return count;
            }
        }

        // if we didn't find a fixed pt on vertices, we subdivide
        vector<Tet*> new_candidates;
        // subdivision will create 8 smaller tetrahedrons and those tetrahedrons also create vertices and edges
        tet->subdivide(time, temp_verts, temp_edges, temp_tris, new_candidates);
        count++;
        if(new_candidates.size() != 8) Utility::throwErrorMessage( "Mesh::find_fixed_pt_location: Error! The size of new_candidates is not 8!");

        for(Tet* new_tet : new_candidates){
            if( !is_candidate_tet(new_tet, time) ){
//                qDebug()<< "invalid"  << new_tet->verts[0]->vel_str(time) << new_tet->verts[1]->vel_str(time) << new_tet->verts[2]->vel_str(time) << new_tet->verts[3]->vel_str(time);
                delete new_tet;
                new_tet = nullptr;
                continue;
            }
            candidates.push(new_tet);
        }


        // get ready for next iteration
        delete tet;
        tet = nullptr;
    }

    qDebug() << "size of candidates" << candidates.size();
    qDebug() << "pop:" << num_pops;
    Utility::clear_mem(temp_verts);
    Utility::clear_mem(temp_edges);
    Utility::clear_mem(temp_tris);
    Utility::clear_mem(candidates);
    if(tet != nullptr) delete tet;

    return count;
}

