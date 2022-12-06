#ifndef STREAMLINE_H
#define STREAMLINE_H

#include <vector>
#include "Geometry/Vertex.h"
#include "Others/Predefined.h"

using namespace std;

class StreamLine
{
public:
    double time; // indicates which time this streamline is for
    vector<Vertex*> verts;

    StreamLine();
    StreamLine(vector<Vertex*> vs, const double& t);
    ~StreamLine();

    inline UL num_verts();
};


inline UL StreamLine::num_verts()
{
    return this->verts.size();
}

#endif // STREAMLINE_H
