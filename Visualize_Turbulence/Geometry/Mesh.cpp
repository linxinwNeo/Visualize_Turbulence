#include "Geometry/Mesh.h"
#include <set>
#include <float.h>

Mesh::Mesh()
{
    this->radius = 0.;
    this->num_time_steps = 0;
    // reserve vector memories to save time
    this->edges.reserve(10000);
    this->tris.reserve(10000);
}


Mesh::~Mesh()
{
    // free memories of each vector
    unsigned long i;
    for( i=0; i<this->num_verts(); i++ ){
        if( this->verts[i] != NULL ){
            delete this->verts[i];
            this->verts[i] = NULL;
        }
    }


    for( i=0; i<this->num_edges(); i++ ){
        if( this->edges[i] != NULL ){
            delete this->edges[i];
            this->edges[i] = NULL;
        }
    }


    for( i=0; i<this->num_tris(); i++ ){
        if( this->tris[i] != NULL ){
            delete this->tris[i];
            this->tris[i] = NULL;
        }
    }


    for( i=0; i<this->num_tets(); i++ ){
        if( this->tets[i] != NULL ){
            delete this->tets[i];
            this->tets[i] = NULL;
        }

    }
    this->verts.clear();
    this->edges.clear();
    this->tris.clear();
    this->tets.clear();
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
    this->radius = length(center - min);
    this->rot_center = center * 1.0;
}

// build unique triangles for the mesh and assign them accordingly to
// the verts, tets
void Mesh::build_triangles()
{
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

        for( Tet* neighbor_Tet : v2->tets ){ // we are certain that the same triangle is shared only by the neighbor tets of this vert
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


void Mesh::max_vor_mag(const unsigned int time, double& min, double& max) const
{
    if(time < 0 || time > this->num_time_steps) return;

    min = DBL_MAX;
    max = DBL_MIN;
    for( const Vertex* v : this->verts ){
        Vector3d vor = v->vors[time];
        double mag = length(vor);
        if(mag < min) min = mag;
        if(mag > max) max = mag;
    }
    // scale
    min = min/2;
    max = max/2;
}


// tet2 can be null in case the incoming triangle is used only by the tet1.
void Mesh::assign_triangle(Tet* tet1, Tet * tet2, Vertex * v1, Vertex * v2, Vertex *v3)
{
    Triangle* new_tri = new Triangle(v1, v2, v3);
    // add triangle to the neighbor tri list of vertices
    v1->add_triangle(new_tri);
    v2->add_triangle(new_tri);
    v3->add_triangle(new_tri);
    // add triangle to the tri list of tets
    tet1->add_triangle(new_tri);
    if(tet2!=NULL) tet2->add_triangle(new_tri);
    // add triangle to the mesh
    this->add_triangle(new_tri);
}



