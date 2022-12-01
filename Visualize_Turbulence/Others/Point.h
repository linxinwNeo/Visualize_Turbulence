#ifndef POINT_H
#define POINT_H

#include "Others/Vector3d.h"


class Point
{
public:
    // variables
    double x;
    double y;
    double z;

    // functions
    inline Point();
    inline Point(const double, const double, const double);
    inline Point(const Vector3d a);
    inline Point(const Vector3d* a);
    inline ~Point();

    inline Point* clone() const; // deep copy

    inline void set_cords(const double, const double, const double);
};


inline Point::Point()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
}


inline Point::Point(const double x, const double y, const double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}


inline Point::Point(const Vector3d a)
{
    this->x = a.entry[0];
    this->y = a.entry[1];
    this->z = a.entry[2];
}


inline Point::Point(const Vector3d *a)
{
    this->x = a->entry[0];
    this->y = a->entry[1];
    this->z = a->entry[2];
}


inline Point::~Point()
{
    return;
}


// deep copy
inline Point* Point::clone() const
{
    Point* newPt = new Point( this->x, this->y, this->z );
    return newPt;
}


inline void Point::set_cords(const double x, const double y, const double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

#endif // POINT_H
