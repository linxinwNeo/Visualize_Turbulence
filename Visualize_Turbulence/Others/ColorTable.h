#ifndef COLORTABLE_H
#define COLORTABLE_H

#include "Others/Vector3d.h"
#include <vector>

// https://www.inf.ed.ac.uk/teaching/courses/vis/lecture_notes/lecture6.pdf

class ColorTable
{
public:
    int num_colors;
    std::vector<Vector3d*> levels;

    ColorTable();
    ~ColorTable();

    Vector3d lookUp(const double);
};

#endif // COLORTABLE_H
