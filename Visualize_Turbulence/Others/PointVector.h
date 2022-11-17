#ifndef POINTVECTOR_H
#define POINTVECTOR_H

#include <vector>
#include "Geometry/Point.h"

class PointVector
{
public:
    // variables
    std::vector<Point *> pts;


    // functions
    inline PointVector();
    ~PointVector();

    inline void add_pt(Point *);
    inline void add_pt(const double x, const double y, const double z);
    bool remove_pt_at_idx( const unsigned long idx ); // return true if success
    PointVector* clone() const; // deep copy
};


inline PointVector::PointVector()
{
    return;
}


inline void PointVector::add_pt(Point* pt)
{
    this->pts.push_back(pt);
}


inline void PointVector::add_pt(const double x, const double y, const double z)
{
    Point* newPt = new Point(x, y, z);
    this->add_pt(newPt);
}

#endif // POINTVECTOR_H
