#include "Edge.h"
#include "Geometry/Vertex.h"
#include "Others/Vector3d.h"

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


// interpolate between two verts of this edge at time t
Vertex* Edge::linear_interpolate_basedOn_vorMag(const double &t, const double &target_val) const
{
    const Vertex* vert1 = verts[0];
    const Vertex* vert2 = verts[1];

    const Vector3d* vor1 = vert1->vors.at(t);
    const Vector3d* vor2 = vert2->vors.at(t);

    const double mag1 = length(vor1);
    const double mag2 = length(vor2);

    if( (target_val < mag1 || target_val > mag2) && (target_val > mag1 || target_val < mag2)  ){
        qDebug() << "Edge::linear_interpolate_basedOn_vorMag: error!, target_val is not correct";
    }

    Vertex* newVertex = new Vertex( vert1->cords );
    const double d_mag = mag2-mag1;
    if(d_mag == 0) return newVertex;
    const double ratio = ( target_val - mag1 ) / d_mag;
    Vector3d new_cord = Vector3d(vert1->cords);

    new_cord = new_cord + (vert2->cords - vert1->cords) * ratio;

    newVertex->cords = new_cord;

    return newVertex;
}
