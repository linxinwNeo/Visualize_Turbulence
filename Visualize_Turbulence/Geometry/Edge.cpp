#include "Edge.h"
#include "Geometry/Vertex.h"
#include "Others/Utilities.h"
#include "Others/Vector3d.h"

Edge::Edge(Vertex* v1, Vertex* v2){
    if(v1 == nullptr || v2 == nullptr) Utility::throwErrorMessage(" Edge::Edge: v1 or v2 is null!");
    this->add_vert(v1);
    this->add_vert(v2);
}

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


// return the middle pt on this edge
Vector3d Edge::middle_pt() const
{
    return (this->verts[0]->cords + this->verts[1]->cords) / 2.;
}


// return the a point at exactly the ratio pt of the edge
Vector3d Edge::near_middle_pt(const double ratio) const
{
    if(ratio >= 0.5) Utility::throwErrorMessage("Edge::random_pt_near_middle_pt: Error! ratio is too big!");
    return this->verts[1]->cords * (1.-ratio) - this->verts[0]->cords * (ratio) + this->verts[0]->cords;
}


bool Edge::has_vert(const Vertex *v) const
{
    if(v==NULL) Utility::throwErrorMessage("Edge::has_vert: v is NULL!");

    if(verts[0] == v) return true;
    else if(verts[1] == v) return true;
    return false;
};


// interpolate between two verts of this edge at time t
Vertex* Edge::linear_interpolate_basedOn_vorMag(const double &t, const double &target_val)
{
    const Vertex* vert1 = verts[0];
    const Vertex* vert2 = verts[1];

    const Vector3d cord1 = vert1->cords;
    const Vector3d cord2 = vert2->cords;

    const Vector3d vel1 = Vector3d(vert1->vels.at(t));
    const Vector3d vel2 = Vector3d(vert2->vels.at(t));

    const Vector3d vor1 = Vector3d(vert1->vors.at(t));
    const Vector3d vor2 = Vector3d(vert2->vors.at(t));

    const double mu1 = vert1->mus.at(t);
    const double mu2 = vert2->mus.at(t);

    const double mag1 = length(vor1);
    const double mag2 = length(vor2);

    if( (target_val < mag1 || target_val > mag2) && (target_val > mag1 || target_val < mag2)  ){
        qDebug() << "Edge::linear_interpolate_basedOn_vorMag: error!, target_val is not correct";
    }

    Vertex* newVertex = new Vertex( cord1 );
    const double d_mag = mag2-mag1;
    if(d_mag == 0) return newVertex;
    const double ratio = ( target_val - mag1 ) / d_mag;

    // interpolate new values
    Vector3d new_cord = cord1 + (cord2 - cord1) * ratio;
    Vector3d* new_vel = new Vector3d( cord1 + (vel2 - vel1) * ratio );
    Vector3d* new_vor = new Vector3d( vor1 + (vor2 - vor1) * ratio );
    double new_mu = mu1 + (mu2 - mu1) * ratio;

    // assign new values
    newVertex->cords = new_cord;
    newVertex->vels[t] = new_vel;
    newVertex->vors[t] = new_vor;
    newVertex->mus[t] = new_mu;

    // assigne parents
    newVertex->add_edge( this );
    return newVertex;
}
