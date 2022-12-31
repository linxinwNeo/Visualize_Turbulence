#include "ECG.h"

void ECG_EDGE::set(Singularity *first, Singularity *second)
{
    this->first = first;
    this->sec = second;
}


ECG::ECG()
{

}


ECG::~ECG()
{
    for(Singularity* sing : this->nodes){
        if(sing!=NULL) delete sing;
    }
    this->nodes.clear();


    for(ECG_EDGE* e : this->edges){
        if(e!=NULL) delete e;
    }

    this->edges.clear();
}
