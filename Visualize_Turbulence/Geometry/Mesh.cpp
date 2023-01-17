#include "Geometry/Mesh.h"
#include "Others/Utilities.h"
#include "Analysis/singularity.h"
#include <set>
#include <float.h>

Mesh::Mesh()
{
    //this->radius = 0.;
    this->num_time_steps = 0;
    // reserve vector memories to save time
    this->edges.reserve(100000);
    this->tris.reserve(100000);
}


Mesh::~Mesh()
{
    UL i, j;

    // clear vertices
    for( i=0; i<this->num_verts(); i++ ){
        if( this->verts[i] != NULL ){
            delete this->verts[i];
            this->verts[i] = NULL;
        }
    }


    // clear edges
    for( i=0; i<this->num_edges(); i++ ){
        if( this->edges[i] != NULL ){
            delete this->edges[i];
            this->edges[i] = NULL;
        }
    }

    // clear tris
    for( i=0; i<this->num_tris(); i++ ){
        if( this->tris[i] != NULL ){
            delete this->tris[i];
            this->tris[i] = NULL;
        }
    }


    // clear tets
    for( i=0; i<this->num_tets(); i++ ){
        if( this->tets[i] != NULL ){
            delete this->tets[i];
            this->tets[i] = NULL;
        }
    }

    // clear memoery used by ECGs
    for(auto& pair: this->ECG_for_all_t){
        ECG* ecg = pair.second;
        delete ecg;
    }


    // clear isosurfaces
    for(auto& pair : isosurfaces_for_all_t){
        Isosurface* isosurf = pair.second;
        delete isosurf;
    }

    // clear streamlines
    for(auto& pair : streamlines_for_all_t){
        vector<StreamLine*> sls = pair.second;
        for(StreamLine* sl : sls){
            delete sl;
        }
    }

    this->verts.clear();
    this->edges.clear();
    this->tris.clear();
    this->tets.clear();
    this->boundary_tris.clear();
    this->min_max_at_verts_for_all_t.clear();
    this->ECG_for_all_t.clear();
    this->streamlines_for_all_t.clear();
}


void Mesh::calc_Bounding_Sphere()
{
    unsigned long i;
    Vector3d min, max;

    for (i=0; i<verts.size(); i++) {
        if (i==0)  {
            min.set(verts[i]->x(), verts[i]->y(), verts[i]->z());
            max.set(verts[i]->x(), verts[i]->y(), verts[i]->z());
        }
        else {
            if (verts[i]->x() < min.entry[0])
                min.entry[0] = verts[i]->x();
            if (verts[i]->x() > max.entry[0])
                max.entry[0] = verts[i]->x();
            if (verts[i]->y() < min.entry[1])
                min.entry[1] = verts[i]->y();
            if (verts[i]->y() > max.entry[1])
                max.entry[1] = verts[i]->y();
            if (verts[i]->z() < min.entry[2])
                min.entry[2] = verts[i]->z();
            if (verts[i]->z() > max.entry[2])
                max.entry[2] = verts[i]->z();
        }
    }
    Vector3d center = (min + max) * 0.5;
    //this->radius = length(center - min);
    this->rot_center = center * 1.0;
}


// build unique triangles for the mesh and assign them accordingly to
// the verts, tets
void Mesh::build_triangles()
{
    this->boundary_tris.reserve(this->num_tets() * 4); // a rough number

    for( Tet* tet : this->tets ) // loop every tet
    {
        if(tet->num_tris() == 4) continue;

        Vertex* v1 = tet->verts[0];
        Vertex* v2 = tet->verts[1];
        Vertex* v3 = tet->verts[2];
        Vertex* v4 = tet->verts[3];
        for( Tet* neighbor_Tet : v1->tets ){ // we are certain that the same triangle is shared only by the neighbor tets of this vert
            if(neighbor_Tet == tet) continue;
            // these are three triangles that can be found by using neighbor tets of v1
            // v1,v2,v3
            // v1,v3,v4
            // v1,v2,v4
            if( neighbor_Tet->has_verts(v1, v2, v3) &&  !neighbor_Tet->has_triangle(v1, v2, v3)) // if this neighbor tet shares the same 3 vertices and does not have corresponding triangle
            {
                this->assign_triangle(tet, neighbor_Tet, v1, v2, v3);
                continue;
            }

            if( neighbor_Tet->has_verts(v1, v3, v4) &&  !neighbor_Tet->has_triangle(v1, v3, v4)) // if this neighbor tet shares the same 3 vertices and does not have corresponding triangle
            {
                this->assign_triangle(tet, neighbor_Tet, v1, v3, v4);
                continue;
            }

            if( neighbor_Tet->has_verts(v1, v2, v4) &&  !neighbor_Tet->has_triangle(v1, v2, v4)) // if this neighbor tet shares the same 3 vertices and does not have corresponding triangle
            {
                this->assign_triangle(tet, neighbor_Tet, v1, v2, v4);
                continue;
            }
        }

        // we are certain that the same triangle is shared only by the neighbor tets of this vert
        for( Tet* neighbor_Tet : v2->tets ){
            if(neighbor_Tet == tet) continue;
            // these are one triangle left that can be found by using neighbor tets of v2
            // v2,v3,v4
            if( neighbor_Tet->has_verts(v2, v3, v4) &&  !neighbor_Tet->has_triangle(v2, v3, v4)) // if this neighbor tet shares the same 3 vertices and does not have corresponding triangle
            {
                this->assign_triangle(tet, neighbor_Tet, v2, v3, v4);
                continue;
            }
        }
    }

    // loop tets again, this time, for those tets that don't have 4 triangles, we add triangles naively
    // because the missing ones will not duplicate with other tets.
    for( Tet* tet : this->tets ){
        if(tet->num_tris() == 4) continue;
        Vertex* v1 = tet->verts[0];
        Vertex* v2 = tet->verts[1];
        Vertex* v3 = tet->verts[2];
        Vertex* v4 = tet->verts[3];
        if(!tet->has_triangle(v1, v2, v3)){
            this->assign_triangle(tet, NULL, v1, v2, v3);
        }
        if(!tet->has_triangle(v1, v2, v4)){
            this->assign_triangle(tet, NULL, v1, v2, v4);
        }
        if(!tet->has_triangle(v1, v3, v4)){
            this->assign_triangle(tet, NULL, v1, v3, v4);
        }
        if(!tet->has_triangle(v2, v3, v4)){
            this->assign_triangle(tet, NULL, v2, v3, v4);
        }
    }
}


// build unique edges for the mesh and assign them accordingly to
// verts, triangles, tets.
// assume triangles are built
void Mesh::build_edges()
{
    for( Triangle* tri : this->tris )
    {
        if(tri->num_edges() == 3) continue;

        Vertex* v1 = tri->verts[0];
        Vertex* v2 = tri->verts[1];
        Vertex* v3 = tri->verts[2];
        if(!tri->has_edge(v1, v2)){ // does not have this edge, then other tris do not have this edge as well
            assign_edge(v1, v2);
        }

        if(!tri->has_edge(v1, v3)){ // does not have this edge, then other tris do not have this edge as well
            assign_edge(v1, v3);
        }

        if(!tri->has_edge(v2, v3)){ // does not have this edge, then other tris do not have this edge as well
            assign_edge(v2, v3);
        }

    }
}


// should be called after tris are generated
// for each tet, add neighbor tets to its neighbor tet list
void Mesh::build_tetNeighbors()
{
    for(Tet* tet : this->tets)
    {
        set<Tet*> uniq_tets;
        for(Triangle* tri : tet->tris){
            for(Tet* tri_tet : tri->tets){
                if(tri_tet != tet) uniq_tets.insert(tri_tet);
            }
        }
        // for each unique tet in the set, add it to the current tet
        // additional checkings
        if(uniq_tets.size() > 4) Utility::throwErrorMessage(QString("build_tetNeighbors: tet %1 tet has more than 4 neighbor tets!").arg(tet->idx));
        if(uniq_tets.size() <= 0) Utility::throwErrorMessage(QString("build_tetNeighbors: tet %1 has less than 1 neighbor tets!").arg(tet->idx));
        for(Tet* uniq_tet : uniq_tets){
            tet->add_tet(uniq_tet);
        }
    }
}


void Mesh::assign_edge(Vertex* v1, Vertex* v2){
    // create edge and add this edge to the mesh
    Edge* e = new Edge(v1, v2);
    this->add_edge(e);

    // add edge to tets
    // add tets to edge
    for(Tet* tet : v1->tets){
        // if this tet has both verts and it does not have this edge
        if( tet->has_vert(v1) && tet->has_vert(v2) && !tet->has_edge(v1,v2) )
        {
            tet->add_edge(e);
            e->add_tet(tet);
            // add this edge to all possible triangles
            for(Triangle* tri : tet->tris){
                // if this triangle has both verts and does not have the edge
                if( tri->has_vert(v1) && tri->has_vert(v2) && !tri->has_edge(v1, v2) )
                {
                    tri->add_edge(e); // add edge to this triangle
                    e->add_triangle(tri); // add this triangle to the edge
                }
            }
        }
    }
}


void Mesh::max_vor_mag(const double time, double& min_vor, double& max_vor) const
{
    if(time < 0 || time > this->num_time_steps) return;

    min_vor = DBL_MAX;
    max_vor = DBL_MIN;
    for( const Vertex* v : this->verts ){
        const Vector3d vor = v->vors.at(time);
        const double mag = length(vor);
        if(mag < min_vor) min_vor = mag;
        if(mag > max_vor) max_vor = mag;
    }

    // shinking the difference
    double diff = max_vor - min_vor;
    diff *= 0.5;
    diff /= 2;
    max_vor -= diff;
    min_vor += diff;
}


void Mesh::max_vel_mag(const double time, double& min_vel, double& max_vel) const
{
    if(time < 0 || time > this->num_time_steps) return;

    min_vel = DBL_MAX;
    max_vel = DBL_MIN;
    for( const Vertex* v : this->verts ){
        Vector3d vel = v->vels.at(time);
        double mag = length(vel);
        if(mag < min_vel) min_vel = mag;
        if(mag > max_vel) max_vel = mag;
    }

    // shinking the difference
    double diff = max_vel-min_vel;
    diff *= 0.5;
    diff /= 2;
    max_vel -= diff;
    min_vel += diff;
}


// tet2 can be null in case the incoming triangle is used only by the tet1.
void Mesh::assign_triangle(Tet* tet1, Tet * tet2, Vertex * v1, Vertex * v2, Vertex *v3)
{
    Triangle* new_tri = new Triangle(v1, v2, v3);
    if(tet2 == NULL) {
        new_tri->is_boundary = true;
        this->boundary_tris.push_back(new_tri);
    }
    // add triangle to the neighbor tri list of vertices
    v1->add_triangle(new_tri);
    v2->add_triangle(new_tri);
    v3->add_triangle(new_tri);
    // add triangle to the tri list of tets
    tet1->add_triangle(new_tri);
    // add tet1 to the triangle
    new_tri->add_tets(tet1);
    if(tet2!=NULL) {
        // add triangle to the tet1
        tet2->add_triangle(new_tri);
        // add tet2 to the triangle
        new_tri->add_tets(tet2);
    }
    // add triangle to the mesh
    this->add_triangle(new_tri);
}


void Mesh::calc_normal_for_all_tris()
{
    for(Triangle* tri : this->tris){
        tri->cal_normal();
    }
}


void Mesh::calc_vor_min_max_at_verts_for_all_t()
{
    double time = 0.;
    while( time < this->num_time_steps - 1. )
    {
        double min = DBL_MAX, max = DBL_MIN;
        for(Vertex* v : verts){
            Vector3d* vor = v->vors.at(time);
            const double mag = length(vor);
            if(mag < min) min = mag;
            if(mag > max) max = mag;
        }
        this->add_vor_min_max_at_verts_for_all_t(time, {min, max});
        time += time_step_size;
    }
}


void Mesh::calc_center_for_all_tet()
{
    for(Tet* tet:tets){
        tet->center = tet->centroid();
    }
}


// filling mesh->ECG_for_all_t
void Mesh::build_ECG_for_all_t()
{
    // calculate singularities for all times
    this->interpolate_vertices_for_all_t();


    unordered_map< double, vector<Singularity*> > map = this->detect_sings();

    double t = 0.;
    while( t < this->num_time_steps - 1. )
    {
        ECG* ecg = new ECG(t);
        // insert singularities for ecg at time t
        vector<Singularity*> sings = map[t];
        qDebug() << "singularity size for time " << t <<  ": " <<  sings.size();
        for(Singularity* sing : sings){
            ecg->add_sing(sing); // add singularity one by one
        }

        qDebug() << "Build ECG nodes";
        ecg->build_ECG_NODES();
        auto seeds = ecg->placing_random_seeds(this, NUM_SEEDS);
        qDebug() << "Build ECG edges";
        ecg->build_ECG_EDGES(this, seeds);
        this->ECG_for_all_t[t] = ecg;

        t += time_step_size;
    }
}


void Mesh::interpolate_vertices_for_all_t()
{
    double t = 0.;
    qDebug() << "Begin interpolate vertices";
    while( t < this->num_time_steps - 1. )
    {
        for(Vertex* vert : this->verts){

            vert->linear_interpolate_vel(t);

            vert->linear_interpolate_vor(t);

            vert->linear_interpolate_mu(t);
        }

        t += time_step_size;
    }
    qDebug() << "Done interpolating vertices";
}


// target is the vertex and we are interested in which tet it is in
// prev_tet is the tet that contains the previous vertex. we should find the tet of target by using the neighbors
// of the start_tet.
// ws contains barycentric coordinate of this pt in that tet if found
// may return a NULL
Tet* Mesh::inWhichTet(const Vector3d& target_pt, Tet* prev_tet, double ws[4]) const
{
    set<Tet*> used;
    Tet* cur_tet = prev_tet;
    // it only breaks if we found the target
    while(!cur_tet->is_pt_in2(target_pt, ws)){ // calculate ds
        if(used.find(cur_tet) != used.end()){ // we find this tet has been used, then we return to avoid infinite loops
            return nullptr;
        }
        used.insert(cur_tet);

        unsigned int min_idx; double min_val;
        Utility::array_min(ws, 4, min_idx, min_val);
        // the pt is not in cur_tet
        // we should move to the neighbor tet whose barycentric coordinate is smallest.
        Vertex* min_vert = cur_tet->verts[min_idx];
        Triangle* exit_tri = NULL;
        for(unsigned int i = 0; i<cur_tet->num_tris(); i++ ){
            // find the triangle that does not have this min_vert
            if(!cur_tet->tris[i]->has_vert(min_vert)){
                exit_tri = cur_tet->tris[i];
                break;
            }
        }
        // if we couldn't find exit_tri or the exit_tri is a dead end, we couldn't proceed
        if(exit_tri == NULL || exit_tri->is_boundary){
            if(exit_tri == NULL) qDebug() << "Mesh::inWhichTet: exit_tri is null!";
            return nullptr;
        }
        // then we set up the next iteration
        for(int i = 0; i < 2; i++){
            if(exit_tri->tets[i] != cur_tet){
                cur_tet = exit_tri->tets[i];
                break;
            }
        }

    }
    return cur_tet;
}
