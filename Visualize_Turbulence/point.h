#ifndef POINT_H
#define POINT_H


class Point
{
public:
    // variables
    double x;
    double y;
    double z;

    // functions
    Point();
    Point(double x, double y, double z);
    ~Point();

    Point* clone(); // deep copy
};

#endif // POINT_H
