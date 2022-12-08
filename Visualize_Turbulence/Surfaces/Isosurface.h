#ifndef ISOSURFACE_H
#define ISOSURFACE_H

#include <vector>
#include "Others/Predefined.h"
#include "Geometry/Triangle.h"


class Isosurface{
public:
    double time; // indicates which time this isosurface is for
    double iso_val; // value of the isosurface
    vector<Triangle*> tris;

    Isosurface();
    ~Isosurface();

    inline UL num_tris() const ;
    inline void add_tri(Triangle* tri);
    inline void add_tri(vector<Triangle*> tris);
};


inline unsigned long Isosurface::num_tris() const
{
    return this->tris.size();
}


inline void Isosurface::add_tri(Triangle *tri)
{
    this->tris.push_back(tri);
}


inline void Isosurface::add_tri(vector<Triangle*> tris)
{
    for(Triangle* tri : tris){
        this->tris.push_back(tri);
    }
}

#endif // ISOSURFACE_H
