#include "ColorTable.h"

ColorTable::ColorTable()
{
    this->num_colors = 3;

    Vector3d* RED = new Vector3d( 1., 0., 0. );
    Vector3d* BLUE = new Vector3d( 0., 0., 1. );
    Vector3d* GREEN = new Vector3d( 0., 1., 0. );

    levels.push_back(BLUE); // lower value has blue color
    levels.push_back(GREEN);
    levels.push_back(RED); // high value has red color
}

ColorTable::~ColorTable()
{
    for(Vector3d* level : levels){
        if(level) delete level;
    }
    this->levels.clear();
}


// assume s is [0, 1], inclusive
Vector3d ColorTable::lookUp( const double s)
{
    // if outside the range, simple return the extreme colors
    if(s <= 0) return Vector3d(this->levels[0]);
    if(s == 0.5) return Vector3d(this->levels[1]);
    if(s >= 1.) return Vector3d(this->levels[2]);

    // if between 0 and 0.5
    if( s > 0 && s < 0.5 )
    {
        const double w1 = s ;
        const double w2 = 0.5 - w1;
        Vector3d color1 = Vector3d(this->levels[0]) * w2;
        Vector3d color2 = Vector3d(this->levels[1]) * w1;
        Vector3d combined = color1 + color2;
        normalize(combined);
        return combined;
    }

    // if between 0.5 and 1
    else
    {
        double w1 = s - 0.5;
        double w2 = 0.5 - w1;
        Vector3d color1 = Vector3d(this->levels[1]) * w2;
        Vector3d color2 = Vector3d(this->levels[2]) * w1;
        Vector3d combined = color1 + color2;
        normalize(combined);
        return combined;
    }
}


