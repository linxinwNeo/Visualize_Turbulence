#include "Lines/PathLine.h"
#include "Others/Predefined.h"

PathLine::PathLine()
{

}

PathLine::PathLine(vector<Vertex*> vs)
{
    this->verts.reserve(vs.size());

    for(auto& v : vs){
        this->verts.push_back(v);
    }
}

PathLine::~PathLine()
{
    for(UL i = 0; i < this->num_verts(); i++){
        if(this->verts[i] != NULL) delete this->verts[i];
    }

    this->verts.clear();
}
