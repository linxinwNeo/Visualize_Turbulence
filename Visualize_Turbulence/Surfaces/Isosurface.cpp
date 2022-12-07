#include "Surfaces/Isosurface.h"


Isosurface::Isosurface()
{
    this->time = 0;
}

Isosurface::~Isosurface()
{
    UL i;
    for(i = 0; i < this->num_tris(); i++){
        Triangle* tri = this->tris[i];
        if(tri != NULL){
            delete tri;
            this->tris[i] = NULL;
        }
    }

    tris.clear();
}
