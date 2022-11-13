#include "pointVector.h"

PointVector::PointVector()
{

}


PointVector::~PointVector()
{
    for( auto & pt : this->pts) {
        delete pt;
    }
    this->pts.clear();
}


void PointVector::add_pt(Point* pt)
{
    this->pts.push_back(pt);
}


void PointVector::add_pt(double x, double y, double z)
{
    Point* newPt = new Point(x, y, z);
    this->add_pt(newPt);
}


bool PointVector::remove_pt_at_idx(unsigned long idx)
{
    // checking if the pt exists
    if( idx < 0 || idx >= this->pts.size() ){
        return false;
    }

    this->pts.erase(this->pts.begin() + idx);
    return true;
}


// deep copy
PointVector *PointVector::clone()
{
    PointVector* newPV = new PointVector();
    // reserve space
    newPV->pts.reserve( this->pts.size() );

    for( auto & pt : this->pts ){
        newPV->pts.push_back( pt->clone() );
    }

    return newPV;
}

