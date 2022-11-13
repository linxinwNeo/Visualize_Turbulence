#ifndef POINTVECTOR_H
#define POINTVECTOR_H

#include <vector>
#include "point.h"

class PointVector
{
public:
    // variables
    std::vector<Point *> pts;


    // functions
    PointVector();
    ~PointVector();

    void add_pt(Point *);
    void add_pt(double x, double y, double z);
    bool remove_pt_at_idx( unsigned long idx ); // return true if success
    PointVector* clone(); // deep copy
};

#endif // POINTVECTOR_H
