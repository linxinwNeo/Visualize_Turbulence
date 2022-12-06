#include "StreamLine.h"

StreamLine::StreamLine()
{
    this->time = 0.;
}


StreamLine::~StreamLine()
{
    UL i;

    if(seed != NULL) {
        delete seed;
        seed = NULL;
    }

    for(i = 0; i < this->num_fw_verts(); i++){
        if(this->fw_verts[i] != NULL) delete this->fw_verts[i];
    }
    this->fw_verts.clear();

    for(i = 0; i < this->num_bw_verts(); i++){
        if(this->bw_verts[i] != NULL) delete this->bw_verts[i];
    }
    this->bw_verts.clear();
}
