#include "ColorTable.h"

ColorTable::ColorTable()
{
    this->num_colors = 3;

    RGB* RED = new RGB( {1., 0., 0.} );
    RGB* GREEN = new RGB( {0., 1., 0.} );
    RGB* BLUE = new RGB( {0., 0., 1.} );

    levels.push_back(BLUE); // lower value has blue color
    levels.push_back(GREEN);
    levels.push_back(RED); // high value has red color
}

ColorTable::~ColorTable()
{
    for(RGB* level : levels){
        if(level){
            delete level;
        }
    }
    this->levels.clear();
}


RGB ColorTable::convert_ptr(RGB* rgb_ptr)
{
    RGB rgb;
    rgb.R = rgb_ptr->R;
    rgb.G = rgb_ptr->G;
    rgb.B = rgb_ptr->B;
    return rgb;
}


// assume s is [0, 1], inclusive
RGB ColorTable::lookUp(double s)
{
    // if outside the range, simple return the extreme colors
    if(s <= 0) return this->convert_ptr(this->levels[0]);
    if(s == 0.5) return this->convert_ptr(this->levels[1]);
    if(s >= 1) return this->convert_ptr(this->levels[2]);

    // if between 0 and 0.5
    if( s > 0 && s < 0.5 )
    {
        double w1 = s * 2.;
        double w2 = 1. - w1;
        RGB color1 = convert_ptr(this->levels[0]);
        RGB color2 = convert_ptr(this->levels[1]);
        color1.R = color1.R*w1;
        color1.G = color1.G*w1;
        color1.B = color1.B*w1;

        color2.R = color2.R*w2;
        color2.G = color2.G*w2;
        color2.B = color2.B*w2;

        RGB combined = { color1.R+color2.R, color1.G+color2.G, color1.B+color2.B };
        return combined;
    }

    // if between 0.5 and 1
    else
    {
        double w1 = (s-0.5) * 2.;
        double w2 = 1. - w1;
        RGB color1 = convert_ptr(this->levels[1]);
        RGB color2 = convert_ptr(this->levels[2]);
        color1.R = color1.R*w1;
        color1.G = color1.G*w1;
        color1.B = color1.B*w1;

        color2.R = color2.R*w2;
        color2.G = color2.G*w2;
        color2.B = color2.B*w2;

        RGB combined = { color1.R+color2.R, color1.G+color2.G, color1.B+color2.B };
        return combined;
    }
}


