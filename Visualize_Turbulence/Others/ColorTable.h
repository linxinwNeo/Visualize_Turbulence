#ifndef COLORTABLE_H
#define COLORTABLE_H

#include <vector>

// https://www.inf.ed.ac.uk/teaching/courses/vis/lecture_notes/lecture6.pdf
struct RGB
{
    double R = 0.;
    double G = 0.;
    double B = 0.;
};

class ColorTable
{
public:
    int num_colors;
    std::vector<RGB*> levels;

    ColorTable();
    ~ColorTable();

    RGB lookUp(double);
    RGB convert_ptr(RGB* rgb_ptr);
};

#endif // COLORTABLE_H
