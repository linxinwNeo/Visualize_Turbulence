#include "Geometry/Tet.h"
#include "Geometry/Vertex.h"
#include "Geometry/Triangle.h"
#include "Geometry/Edge.h"
#include "Others/Utilities.h"
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
    this->marching_idices.clear();
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


// assume v is inside this tet
// Using barycentric interpolation scheme, calculate the new Vertex at pt's position
// weights are calculated already
Vertex* Tet::get_vert_at(const Vector3d& v, const double time, double ws[4], bool cal_ws)
{
    // only calculate ws if cal_ws is set to true
    if(cal_ws) this->bary_tet(v, ws);

    Vertex* pt_vert = new Vertex(v);
    pt_vert->add_tet(this);

    Vector3d vel ,  vor;
    double mu = 0.;
    for( unsigned short i = 0; i < this->verts.size(); i++ ){
        Vertex* vert = this->verts[i];
        if(vert == NULL) throwErrorMessage( QString("Tet::interpolate: a null pointer inside vs! Current tet is %1").arg(this->idx) );

        Vector3d* temp_vel = NULL, *temp_vor = NULL;
        double temp_mu = 0.;
        // interpolate velocity
        if(vert->has_vel_at_t(time)) // the velcity is defined at time t for this vertex
        {
            temp_vel = vert->vels.at(time);
        }
        else
        {
            // velocity not defined at time t
            // we need to use linear interpolation to find the value for this vertex between two times
            temp_vel = vert->linear_interpolate_vel(time); // this vel vector has been auto added to v
        }

        // interpolate vorticity
        if(vert->has_vor_at_t(time)) // the vorticity is defined at time t for this vertex
        {
            temp_vor = vert->vors.at(time);
        }
        else
        {
            // the vorticity is not defined at time t fo v
            temp_vor = vert->linear_interpolate_vor(time);
        }

        // interpolate mu
        if(vert->has_mu_at_t(time)) // the vorticity is defined at time t for this vertex
        {
            temp_mu = vert->mus.at(time);
        }
        else
        {
            // the vorticity is not defined at time t fo v
            temp_mu = vert->linear_interpolate_mu(time);
        }

        vel =  vel + (*temp_vel) * ws[i];
        vor =  vor + (*temp_vor) * ws[i];
        mu = mu + temp_mu * ws[i];
    }

    pt_vert->set_vel(time, new Vector3d(vel) ); // adding new vel in order to avoid double deletion
    pt_vert->set_vor(time, new Vector3d(vor) ); // adding new vor in order to avoid double deletion
    pt_vert->set_mu(time, mu);

    return pt_vert;
}


// calculate the volume of this tet
// https://en.wikipedia.org/wiki/Tetrahedron#Volume
double Tet::volume() const
{
    Vector3d a = this->verts[0]->cords;
    Vector3d b = this->verts[1]->cords;
    Vector3d c = this->verts[2]->cords;
    Vector3d d = this->verts[3]->cords;
    Vector3d crossP = cross( b-d, c-d );
    double dotP = dot( a-d, crossP);
    dotP = abs(dotP);
    double volume = ((double) dotP)/6.;
    return volume;
}


// returns the center point of the tet
Vector3d Tet::centroid() const
{
    Vector3d center_cords;
    for(UL i = 0; i < this->num_verts(); i++){
        center_cords += this->verts[i]->cords;
    }
    center_cords = center_cords / 4.;

    return center_cords;
}


// https://stackoverflow.com/questions/25179693/how-to-check-whether-the-point-is-in-the-tetrahedron-or-not
// check if the pt is on the same side of other vertices
bool is_same_side(const Vector3d &v1, const Vector3d &v2, const Vector3d &v3, const Vector3d &v4, const Vector3d &p)
{
    Vector3d normal = cross(v2-v1, v3-v1);
    return dot(normal, v4-v1) * dot(normal, p-v1) >= 0;
}


// https://stackoverflow.com/questions/25179693/how-to-check-whether-the-point-is-in-the-tetrahedron-or-not
// check is parameter point is in this tet
bool Tet::is_pt_in(const Vector3d& v) const
{
    const Vector3d v1 = this->verts[0]->cords;
    const Vector3d v2 = this->verts[1]->cords;
    const Vector3d v3 = this->verts[2]->cords;
    const Vector3d v4 = this->verts[3]->cords;
    const Vector3d p = Vector3d(v);

    return  is_same_side(v1, v2, v3, v4, p) &&
            is_same_side(v2, v3, v4, v1, p) &&
            is_same_side(v3, v4, v1, v2, p) &&
            is_same_side(v4, v1, v2, v3, p);
}


// https://www.researchgate.net/publication/226339789_Virtual_Reality-Based_Interactive_Scientific_Visualization_Environments
// using barycentric cords
bool Tet::is_pt_in2(const Vector3d& p, double ds[4]) const
{
    this->bary_tet(p, ds);
    for(int i = 0; i<4; i++ ){
        if(ds[i] < 0) return false;
    }
    return true;
}


// find the missing v4
Vertex* Tet::missing_vertex(Vertex* v1, Vertex*v2, Vertex* v3)
{
    set<Vertex*> vs{this->verts[0], this->verts[1], this->verts[2], this->verts[3]};

    vs.erase(v1);
    vs.erase(v2);
    vs.erase(v3);

    return *vs.begin();
}


// https://math.stackexchange.com/questions/183030/given-a-tetrahedron-how-to-find-the-outward-surface-normals-for-each-side
// compute the normal vector of the specfied triangle
Vector3d Tet::actual_normal_of( unsigned short tri_idx )
{
    Triangle* tri = this->tris[tri_idx];
    const Vector3d& p = tri->verts[0]->cords;
    Vector3d v4 = missing_vertex(tri->verts[0], tri->verts[1], tri->verts[2])->cords;
    Vector3d normal = tri->normal; // the triangle's normal, maybe opposite, but already normalized
    Vector3d vp = v4 - p;
    if(dot(vp, normal) > 0) // the normal is facing inward, flip it
    {
        normal = normal * (-1.);
    }
    // else: the normal is facing outward, don't need to do anthing
    return normal;
}


// https://stackoverflow.com/questions/38545520/barycentric-coordinates-of-a-tetrahedron
double Tet::ScTP(const Vector3d &a, const Vector3d &b, const Vector3d &c) const
{
    // computes scalar triple product
    return dot(a, cross(b, c));
}


// https://stackoverflow.com/questions/38545520/barycentric-coordinates-of-a-tetrahedron
// the weights are saved in vs[4];
void Tet::bary_tet(const Vector3d & p, double ds[4]) const
{
    const Vector3d& a = this->verts[0]->cords;
    const Vector3d& b = this->verts[1]->cords;
    const Vector3d& c = this->verts[2]->cords;
    const Vector3d& d = this->verts[3]->cords;

    const Vector3d vap = p - a;
    const Vector3d vbp = p - b;

    const Vector3d vab = b - a;
    const Vector3d vac = c - a;
    const Vector3d vad = d - a;

    const Vector3d vbc = c - b;
    const Vector3d vbd = d - b;
    // ScTP computes the scalar triple product
    const double va6 = ScTP(vbp, vbd, vbc);
    const double vb6 = ScTP(vap, vac, vad);
    const double vc6 = ScTP(vap, vad, vab);
    const double vd6 = ScTP(vap, vab, vac);
    const double v6 = 1 / ScTP(vab, vac, vad);
    ds[0] = va6*v6;
    ds[1] = vb6*v6;
    ds[2] = vc6*v6;
    ds[3] = vd6*v6;
    return;
}


// assume vertices->is_above_surface are calculated
void Tet::calc_marching_indices()
{
    double time = 0.;
    const unsigned char one = 0b01;
    while(time < mesh->num_time_steps - 1.)
    {
        this->marching_idices[time] = 0;
        for( unsigned int i = 0; i< this->verts.size(); i++ ){
            const Vertex* vert = verts[i];
            bool is_above = vert->is_above_surface.at(time);
            if(is_above) // if it is above, we set the bit
                this->marching_idices[time] = this->marching_idices.at(time) | (one << i);
        }
        time += time_step_size;
    }
}




// http://paulbourke.net/geometry/polygonise/
vector<Triangle *> Tet::create_isosurface_tris(const double& time)
{
    vector<Triangle*> new_tris;
    // 7 cases
    switch(this->marching_idices.at(time)){
    case 0b0001:
    case 0b1110:{ // the first vert is different than others
        new_tris.push_back(create_isosurface_tris_case1234(verts[0], time));
        break;
    }
    case 0b0010:
    case 0b1101:{ // the second vert is different than others
        new_tris.push_back(create_isosurface_tris_case1234(verts[1], time));
        break;
    }
    case 0b0100:
    case 0b1011:{ // the third vert is different than others
        new_tris.push_back(create_isosurface_tris_case1234(verts[2], time));
        break;
    }
    case 0b1000:
    case 0b0111:{ // the forth vert is different than others
        new_tris.push_back(create_isosurface_tris_case1234(verts[3], time));
        break;
    }
    case 0b0011:
    case 0b1100:{ // a cut between nodes 12 and 34, two triangles
        vector<Triangle*> temp = create_isosurface_tris_case567(verts[0], verts[1], time);
        new_tris.push_back(temp[0]); new_tris.push_back(temp[1]);
        break;
    }
    case 0b0101:
    case 0b1010:{ // a cut between nodes 13 and 24, two triangles
        vector<Triangle*> temp = create_isosurface_tris_case567(verts[0], verts[2], time);
        new_tris.push_back(temp[0]); new_tris.push_back(temp[1]);
        break;
    }
    case 0b0110:
    case 0b1001:{ // a cut between nodes 14 and 23,, two triangles
        vector<Triangle*> temp = create_isosurface_tris_case567(verts[0], verts[3], time);
        new_tris.push_back(temp[0]); new_tris.push_back(temp[1]);
        break;
    }
    }

    return new_tris;
}


// v is the one on the different level than other 3 in the tet
Triangle *Tet::create_isosurface_tris_case1234(const Vertex *v, const double time)
{
    vector<Vertex*> newVerts;
    for(Edge* e : edges){
        if(e->has_vert(v)){
            Vertex* newVert = e->linear_interpolate_basedOn_vorMag(time, surface_level);
            newVert->add_tet(this);
            newVerts.push_back(newVert);
        }
    }
    Triangle* new_tri = new Triangle(newVerts[0], newVerts[1], newVerts[2]);
    return new_tri;
}


struct VertOnEdge{
    Edge* edge;
    Vertex* vert;
};


inline bool is_IDP_edges(Edge* e1, Edge* e2){
    Vertex* v1 = e1->verts[0];
    Vertex* v2 = e1->verts[1];
    Vertex* v3 = e2->verts[0];
    Vertex* v4 = e2->verts[1];
    return (v1 != v3 && v1 != v4 && v2 != v3 && v2 != v4);
}

// v1v2 are the two on the different level than other 2 in the tet
// will create two triangles in this case
vector<Triangle *> Tet::create_isosurface_tris_case567(const Vertex *v1, const Vertex *v2, const double time)
{
    vector<VertOnEdge> newPairs;
    for(Edge* e : edges){
        bool has_v1 = e->has_vert(v1);
        bool has_v2 = e->has_vert(v2);
        if(has_v1 && has_v2) continue;

        if(has_v1 || has_v2){
            Vertex* newVert = e->linear_interpolate_basedOn_vorMag(time, surface_level);
            newVert->add_tet(this);
            VertOnEdge newPair = {e, newVert};
            newPairs.push_back(newPair);
        }
    }

    // looking for 'independent' verts
    // independent := two verts do not share original verts
    short idpVert1 = -1, idpVert2 = -1;
    for( UI i = 0; i < newPairs.size(); i++ ){
        VertOnEdge pair1 = newPairs[i];
        for( UI j = 0; j < newPairs.size(); j++){
            VertOnEdge pair2 = newPairs[j];
            if(is_IDP_edges(pair1.edge, pair2.edge)){
                idpVert1 = i;
                idpVert2 = j;
            }
        }
    }

    short vert3 = -1, vert4 = -1;
    for(UI i = 0; i < newPairs.size(); i++){
        if(i != idpVert1 && i != idpVert2){
            if(vert3 == -1) vert3 = i;
            else{
                vert4 = i;
            }
        }
    }

    Triangle* new_tri1 = new Triangle(newPairs[idpVert1].vert, newPairs[idpVert2].vert, newPairs[vert3].vert);
    Triangle* new_tri2 = new Triangle(newPairs[idpVert1].vert, newPairs[idpVert2].vert, newPairs[vert4].vert);
    vector<Triangle*> newTris = {new_tri1, new_tri2};
    return newTris;
}
