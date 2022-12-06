#include "StreamLine.h"

StreamLine::StreamLine()
{
    this->time = 0.;
}

StreamLine::StreamLine(vector<Vertex*> vs, const double& t)
{
    this->time = t;
    this->verts.reserve(vs.size());

    for(auto& v : vs){
        this->verts.push_back(v);
    }
}

StreamLine::~StreamLine()
{
    for(UL i = 0; i < this->num_verts(); i++){
        if(this->verts[i] != NULL) delete this->verts[i];
    }

    this->verts.clear();
}
