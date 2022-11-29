#include "Edge.h"

bool Edge::is_same(const Edge* edge) const
{
    if(this->verts[0] == edge->verts[0] && this->verts[1] == edge->verts[1]){
        return true;
    }
    else if(this->verts[0] == edge->verts[1] && this->verts[1] == edge->verts[0]){
        return true;
    }
    return false;
}
