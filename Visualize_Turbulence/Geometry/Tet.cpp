#include "Geometry/Tet.h"
#include "Geometry/Vertex.h"
#include "Geometry/Triangle.h"
#include "Geometry/Edge.h"
#include <math.h>
#include <QDebug>

Tet::Tet()
{
    this->idx = -1; // use -1 to indicate the obj is new
    // reserve memories
    this->verts.reserve(4); // a tet can only have exact 4 vertices
    this->edges.reserve(4); // a tet can only have exact 4 edges
    this->tris.reserve(4); // a tet can only have exact 4 faces
    this->tets.reserve(4);  // a tet can only have 4 neighbor tets
}


Tet::Tet(Vertex *v1, Vertex *v2, Vertex *v3,Vertex *v4)
{
    this->add_vert(v1);
    this->add_vert(v2);
    this->add_vert(v3);
    this->add_vert(v4);
}

Tet::Tet(Triangle* tri, Vertex *v4)
{
    this->add_vert(tri->verts[0]);
    this->add_vert(tri->verts[1]);
    this->add_vert(tri->verts[2]);
    this->add_vert(v4);
}


Tet::~Tet()
{
    this->verts.clear();
    this->edges.clear();
    this->tris.clear();
    this->tets.clear();
}


bool Tet::has_vert(const Vertex * v1) const
{
    for(Vertex* v : this->verts ){
        if(v == v1) return true;
    }
    return false;
}


bool Tet::has_verts(const Vertex* v1, const Vertex* v2, const Vertex* v3) const
{
    if( this->has_vert(v1) && this->has_vert(v2) && this->has_vert(v3) ) return true;
    else return false;
}


bool Tet::has_triangle(const Vertex* v1, const Vertex* v2, const Vertex* v3) const
{
    for(Triangle* tri : this->tris ){
        if( tri->has_vert(v1) && tri->has_vert(v2) && tri->has_vert(v3) ) return true;
    }
    return false;
}


bool Tet::has_edge(const Vertex *v1, const Vertex *v2) const
{
    for(Edge* e : this->edges){
        bool has_v1 = false, has_v2 = false;
        Vertex* vv1 = e->verts[0];
        Vertex* vv2 = e->verts[1];
        if(vv1 == v1 || vv2 == v1){
            has_v1 = true;
        }
        if(vv1 == v2 || vv2 == v2){
            has_v2 = true;
        }
        if(has_v1 && has_v2) return true;
    }
    return false;
}


// https://stackoverflow.com/questions/25179693/how-to-check-whether-the-point-is-in-the-tetrahedron-or-not
// check if the pt is on the same side of other vertices
bool Tet::is_same_side(const Vector3d &v1, const Vector3d &v2, const Vector3d &v3, const Vector3d &v4, const Vector3d &p) const
{
    Vector3d normal = cross(v2 - v1, v3 - v1);
    double dotV4 = dot(normal, v4 - v1);
    double dotP = dot(normal, p - v1);
    return signbit(dotV4) == signbit(dotP);
}


// https://stackoverflow.com/questions/25179693/how-to-check-whether-the-point-is-in-the-tetrahedron-or-not
// check is parameter point is in this tet
bool Tet::is_pt_in(const Point *pt) const
{
    Vector3d v1 = this->verts[0]->cordsVect();
    Vector3d v2 = this->verts[1]->cordsVect();
    Vector3d v3 = this->verts[2]->cordsVect();
    Vector3d v4 = this->verts[3]->cordsVect();
    Vector3d p = Vector3d(pt->x, pt->y, pt->z);

    return  this->is_same_side(v1, v2, v3, v4, p) &&
            this->is_same_side(v2, v3, v4, v1, p) &&
            this->is_same_side(v3, v4, v1, v2, p) &&
            this->is_same_side(v4, v1, v2, v3, p);
}


// assume pt is inside this tet
// Using barycentric interpolation scheme, calculate the new Vertex at pt's position
Vertex* Tet::interpolate(const Point* pt, const unsigned int time)
{
    Vertex* pt_vert = new Vertex(pt->x, pt->y, pt->z);
    pt_vert->add_tet(this);

    // calculate weights
    // weights are calculated by using the volume ratio
    // we need to divide the tetrahedron into 4 smaller tets
    Triangle* tri1 = this->tris[0];
    Triangle* tri2 = this->tris[1];
    Triangle* tri3 = this->tris[2];
    Triangle* tri4 = this->tris[3];

    Tet tet1 = Tet(tri1, pt_vert);
    Tet tet2 = Tet(tri2, pt_vert);
    Tet tet3 = Tet(tri3, pt_vert);
    Tet tet4 = Tet(tri4, pt_vert);

    vector<double> vols;
    vols.push_back(tet1.volume());
    vols.push_back(tet2.volume());
    vols.push_back(tet3.volume());
    vols.push_back(tet4.volume());

    double total_vol = 0.;
    for( const auto& vol : vols ){
        total_vol += vol;
    }
    if(total_vol == 0.){
        qDebug() << "Tet " << this->idx<< "has volume of 0.0 m^3";
    }

    // calculate the weight for each vertex in tet
    vector<double> weights;
    for( const auto& vol : vols ){
        weights.push_back( vol / total_vol );
    }

    vector<Vertex*> vs;
    vs.push_back( tri1->not_has_vert(this->verts) );
    vs.push_back( tri2->not_has_vert(this->verts) );
    vs.push_back( tri3->not_has_vert(this->verts) );
    vs.push_back( tri4->not_has_vert(this->verts) );

    Vector3d vel, vor;
    double mu = 0.;
    for( int i = 0; i < 4; i++ ){
        if(vs[i] == NULL)
        {
            qDebug() << "Vertex* Tet::interpolate: a null pointer!";
            continue;
        }

        Vector3d temp_vel = Vector3d(vs[i]->vels[time]);
        Vector3d temp_vor = Vector3d(vs[i]->vors[time]);
        vel =  vel + temp_vel * weights[i];
        vor =  vor + temp_vor * weights[i];
        mu = mu + vs[i]->mus[time] * weights[i];
    }

    pt_vert->vels.push_back( new Vector3d(vel) );
    pt_vert->vors.push_back( new Vector3d(vor) );
    pt_vert->mus.push_back(mu);

    return pt_vert;
}


// calculate the volume of this tet
// https://en.wikipedia.org/wiki/Tetrahedron#Volume
double Tet::volume() const
{
    Vector3d a = this->verts[0]->cordsVect();
    Vector3d b = this->verts[1]->cordsVect();
    Vector3d c = this->verts[2]->cordsVect();
    Vector3d d = this->verts[3]->cordsVect();
    Vector3d crossP = cross( b-d, c-d );
    double dotP = dot( a-d, crossP);
    dotP = abs(dotP);
    double volume = ((double) dotP)/6.;
    return volume;
}


// returns the center point of the tet
Point Tet::centroid() const
{
    Vector3d center_cord;
    for(UL i = 0; i < this->num_verts(); i++){
        center_cord += this->verts[i]->cordsVect();
    }
    center_cord = center_cord / 4.;

    return Point(center_cord);
}


