#include "Geometry/Mesh.h"

Mesh::Mesh()
{
    this->radius = 0.;
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
