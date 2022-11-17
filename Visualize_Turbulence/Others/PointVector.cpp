#include "PointVector.h"




PointVector::~PointVector()
{
    for( auto & pt : this->pts) {
        delete pt;
    }
    this->pts.clear();
}


bool PointVector::remove_pt_at_idx(const unsigned long idx)
{
    // checking if the pt exists
    if( idx < 0 || idx >= this->pts.size() ){
        return false;
    }

    this->pts.erase(this->pts.begin() + idx);
    return true;
}


// deep copy
PointVector *PointVector::clone() const
{
    PointVector* newPV = new PointVector();
    // reserve space
    newPV->pts.reserve( this->pts.size() );

    for( auto & pt : this->pts ){
        newPV->pts.push_back( pt->clone() );
    }

    return newPV;
}

