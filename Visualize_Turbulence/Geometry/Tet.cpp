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
    marked = false;

    // reserve memories
    this->verts.reserve(4); // a tet can only have exact 4 vertices
    this->edges.reserve(4); // a tet can only have exact 4 edges
    this->tris.reserve(4); // a tet can only have exact 4 faces
    this->tets.reserve(4);  // a tet can only have 4 neighbor tets
}


Tet::Tet(Vertex *v1, Vertex *v2, Vertex *v3,Vertex *v4)
{
    this->marked = false;
    this->add_vert(v1);
    this->add_vert(v2);
    this->add_vert(v3);
    this->add_vert(v4);
    this->center = this->centroid();
}


Tet::Tet(Triangle* tri, Vertex *v4)
{
    this->marked = false;
    this->add_vert(tri->verts[0]);
    this->add_vert(tri->verts[1]);
    this->add_vert(tri->verts[2]);
    this->add_vert(v4);
    this->center = this->centroid();
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

bool Tet::has_boundary_tri() const
{
    for(Triangle* tri : this->tris){
        if(tri->is_boundary) return true;
    }
    return false;
}


// assume v is inside this tet
// Using barycentric interpolation scheme, calculate the new Vertex at pt's position
// if cal_ws is true, then we calculate the weights
Vertex* Tet::get_vert_at(const Vector3d& v, const double time, double ws[4], bool cal_ws, bool add_this_tet)
{
    // only calculate ws if cal_ws is set to true
    if(cal_ws) this->bary_cords(ws, v);

    Vertex* pt_vert = new Vertex(v);
    if(add_this_tet) pt_vert->add_tet(this);

    Vector3d vel,  vor;
    double mu = 0.;

    for( unsigned short i = 0; i < 4; i ++ ){
        Vertex* vert = this->verts[i];
        double weight = ws[i];

        if(vert == NULL) Utility::throwErrorMessage( QString("Tet::interpolate: a null pointer inside vs! Current tet is %1").arg(this->idx) );

        if(vert->has_vel_at_t(time)){
            Vector3d temp_vel = vert->vels[time];
            vel =  vel + temp_vel * weight;
        }

        if(vert->has_vor_at_t(time)){
            Vector3d temp_vor = vert->vors[time];
            vor = vor + temp_vor * weight;
        }

        if(vert->has_mu_at_t(time)){
            double temp_mu = vert->mus[time];
            mu = mu + temp_mu * weight;
        }
    }

    pt_vert->cords = v;
    pt_vert->set_vel(time, new Vector3d(vel) ); // adding new vel in order to avoid double deletion
    pt_vert->set_vor(time, new Vector3d(vor) ); // adding new vor in order to avoid double deletion
    pt_vert->set_mu(time, mu);

    return pt_vert;
}

Vertex *Tet::get_vert_at(const double time, double ws[4]) const
{
    Vector3d vel,  vor;
    double mu = 0.;
    double x=0, y=0, z = 0;
    for(unsigned short i = 0; i < 4; i++){
        Vertex* vert = this->verts[i];
        double weight = ws[i];

        if(vert->has_vel_at_t(time)){
            Vector3d temp_vel = vert->vels[time];
            vel =  vel + temp_vel * weight;
        }

        if(vert->has_vor_at_t(time)){
            Vector3d temp_vor = vert->vors[time];
            vor = vor + temp_vor * weight;
        }

        if(vert->has_mu_at_t(time)){
            double temp_mu = vert->mus[time];
            mu = mu + temp_mu * weight;
        }

        x += vert->x() * weight;
        y += vert->y() * weight;
        z += vert->z() * weight;
    }

    Vertex* pt_vert = new Vertex();
    pt_vert->set_vel(time, new Vector3d(vel) ); // adding new vel in order to avoid double deletion
    pt_vert->set_vor(time, new Vector3d(vor) ); // adding new vor in order to avoid double deletion
    pt_vert->set_mu(time, mu);
    pt_vert->set_cords(x, y, z);
    return pt_vert;
}


// calculate the volume of this tet
// https://en.wikipedia.org/wiki/Tetrahedron#Volume
double Tet::volume() const
{
    const Vector3d a = this->verts[0]->cords;
    const Vector3d b = this->verts[1]->cords;
    const Vector3d c = this->verts[2]->cords;
    const Vector3d d = this->verts[3]->cords;
    const Vector3d crossP = cross( b-d, c-d );
    double dotP = dot( a-d, crossP);
    dotP = abs(dotP);
    const double volume = ((double) dotP)/6.;
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

// deep_copy the current tet's vertex and edges
Tet *Tet::clone(const double time) const
{
    Vertex* newV1 = this->verts[0]->clone(time, true);
    Vertex* newV2 = this->verts[1]->clone(time, true);
    Vertex* newV3 = this->verts[2]->clone(time, true);
    Vertex* newV4 = this->verts[3]->clone(time, true);
    Tet* newTet = new Tet(newV1, newV2, newV3, newV4);
    return newTet;
}


// find the missing vertex of this tet that is not in the parameters
Vertex* Tet::missing_vertex(Vertex* v1, Vertex*v2, Vertex* v3) const
{
    set<Vertex*> vs{this->verts[0], this->verts[1], this->verts[2], this->verts[3]};

    vs.erase(v1);
    vs.erase(v2);
    vs.erase(v3);

    if(vs.size() != 1){
        Utility::throwErrorMessage("Tet::missing_vertex: couldn't find the missing vertex!");
    }

    return *vs.begin();
}

bool Tet::is_pt_in2(const Vector3d& p, double ds[4]) const
{
    this->bary_tet(p, ds);
    for(int i = 0; i < 4; i ++ ){
        if(ds[i] < 0) return false;
    }

    return true;
}

// checking if the incoming pt is inside this tetrahedron
// by calculating barycentric coordinates and checking the positivity of each
// https://people.sc.fsu.edu/~jburkardt/presentations/cg_lab_barycentric_tetrahedrons.pdf
bool Tet::is_pt_inside(const Vector3d& P, const bool calc_ws, double ws[4]) const
{
    // calculate ws if needed
    if(calc_ws) this->bary_cords(ws, P);

    for(unsigned char i = 0; i < 4; i ++){
        if(ws[i] < -zero_threshold/3.) return false;
    }

    return true;
}


// calculate the barycentric coordinates and saved in ws[4]
// https://people.sc.fsu.edu/~jburkardt/presentations/cg_lab_barycentric_tetrahedrons.pdf
void Tet::bary_cords(double ws[4], const Vector3d& P) const
{
    vector<Triangle*> triangles;
    triangles.reserve(4);

    for(unsigned char i = 0; i < 4; i++){
        Vertex* v = this->verts[i];
        // find the corresponding trianlge of v
        for(unsigned char j = 0; j < 4; j++){
            Triangle* tri = this->tris[j];
            if(!tri->has_vert(v)) triangles.push_back(tri);
        }
    }

    if(triangles.size()!=4) Utility::throwErrorMessage("Tet::bary_cords(double ws[4], const Vector3d P): failed to find corresponding triangle for verts");

    const double dist_P_bcd = Utility::SingedDistance(P, triangles[0]->verts[0]->cords, triangles[0]->verts[1]->cords, triangles[0]->verts[2]->cords);
    const double dist_P_acd = Utility::SingedDistance(P, triangles[1]->verts[0]->cords, triangles[1]->verts[1]->cords, triangles[1]->verts[2]->cords);
    const double dist_P_abd = Utility::SingedDistance(P, triangles[2]->verts[0]->cords, triangles[2]->verts[1]->cords, triangles[2]->verts[2]->cords);
    const double dist_P_abc = Utility::SingedDistance(P, triangles[3]->verts[0]->cords, triangles[3]->verts[1]->cords, triangles[3]->verts[2]->cords);

    const double dist_a_bcd = Utility::SingedDistance(this->verts[0]->cords, triangles[0]->verts[0]->cords, triangles[0]->verts[1]->cords, triangles[0]->verts[2]->cords);
    const double dist_b_acd = Utility::SingedDistance(this->verts[1]->cords, triangles[1]->verts[0]->cords, triangles[1]->verts[1]->cords, triangles[1]->verts[2]->cords);
    const double dist_c_abd = Utility::SingedDistance(this->verts[2]->cords, triangles[2]->verts[0]->cords, triangles[2]->verts[1]->cords, triangles[2]->verts[2]->cords);
    const double dist_d_abc = Utility::SingedDistance(this->verts[3]->cords, triangles[3]->verts[0]->cords, triangles[3]->verts[1]->cords, triangles[3]->verts[2]->cords);

    ws[0] = dist_P_bcd / dist_a_bcd;
    ws[1] = dist_P_acd / dist_b_acd;
    ws[2] = dist_P_abd / dist_c_abd;
    ws[3] = dist_P_abc / dist_d_abc;

    return;
}


// https://math.stackexchange.com/questions/183030/given-a-tetrahedron-how-to-find-the-outward-surface-normals-for-each-side
// compute the normal vector of the specfied triangle
Vector3d Tet::actual_normal_of( unsigned char tri_idx ) const
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


// assume vertices->is_above_surface are calculated
void Tet::calc_marching_indices(unsigned int num_time_steps)
{
    double time = 0.;
    const unsigned char one = 0b01;
    while(time < num_time_steps - 1.)
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
            Vertex* newVert = e->linear_interpolate_basedOn_vorMag(time, surface_level_vals.at(time));
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
            Vertex* newVert = e->linear_interpolate_basedOn_vorMag(time, surface_level_vals.at(time));
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


// create 6 unique edges for this tetrahedron
void Tet::make_edges()
{
    UI i, j;
    for(i = 0; i < 4; i++){
        Vertex* v1 = verts[i];
        for(j = i+1; j < 4; j++){
            Vertex* v2 = verts[j];
            Edge* e = new Edge(v1, v2);
            e->add_tet(this);
            v1->add_edge(e);
            v2->add_edge(e);
            this->add_edge(e);
        }
    }
}


// create triangles
// assume vertices are inserted
void Tet::make_triangles()
{
    Vertex* v1 = this->verts[0];
    Vertex* v2 = this->verts[1];
    Vertex* v3 = this->verts[2];
    Vertex* v4 = this->verts[3];

    Triangle* tri1 = new Triangle(v1, v2, v3);
    Triangle* tri2 = new Triangle(v2, v3, v4);
    Triangle* tri3 = new Triangle(v1, v3, v4);
    Triangle* tri4 = new Triangle(v1, v2, v4);

    this->add_triangle(tri1);
    this->add_triangle(tri2);
    this->add_triangle(tri3);
    this->add_triangle(tri4);

    // may need edges later
}


/* we subdivide the tetrahedron into 4 + 4 = 8 tetrahedrons
 * steps:
 * 1. calculate middle points on 6 edges
 * 2. create 4 smaller tetrahedrons using original 4 vertices
 * 3. identify the edges for octahedron (new edges created by step 2.)
 * 4. identify the major diagonal edge (there are three possible diagonal edges, we just need one)
 * 5. separate the octahedron into 4 tetrahedrons by using the diagonal edge.
 * for newly created vertices, edges, tris, and tets, we append them to the parameter vectors.
*/
void Tet::subdivide(const double time, vector<Vertex*>& new_verts, vector<Edge*>& new_edges, vector<Triangle*>& new_tris, vector<Tet*>& new_tets)
{
    // check if this tet has vertices
    if(this->num_verts() != 4) return;

    /* check if this tet has edges
     * if not, create new edges and add these edges into new_edges
    */
    if(this->num_edges() == 0) {
        this->make_edges();
        for(Edge* e:this->edges) new_edges.push_back(e);
    }

    /* check if this tet has triangles
     * if not, create new triangle and add these triangles into new_tris
    */
    if(this->num_tris() == 0) {
        this->make_triangles();
        for(Triangle* tri : this->tris) new_tris.push_back(tri);
    }

    // copy vertices
    double ws[4];
    unordered_map<Vertex*, Vertex*> vert_copies; vert_copies.reserve(4);
    for(short i = 0; i < 4; i++) vert_copies[ verts[i] ] = get_vert_at(verts[i]->cords, time, ws, true, false);

    // find edge middle points
    // keep track of newly created vertices
    // these are actually the vertices of octahedron
    vector<Vertex*> uniq_edge_points;
    uniq_edge_points.reserve(6);

    vector<Edge*> uniq_new_edges;
    uniq_new_edges.reserve(12+12+1); // must be 25 new edges in total

    set<Edge*> octa_edges;

    // for each edge of the tetrahedron, make unique edge middle vertices
    for( Edge* e : this->edges ){
        Vertex* v1 = vert_copies[e->verts[0]]; // copied vertex
        Vertex* v2 = vert_copies[e->verts[1]]; // copied vertex

        Vector3d middle_cords = e->middle_pt();
        Vertex* new_vert = this->get_vert_at(middle_cords, time, ws, true, false);
        uniq_edge_points.push_back(new_vert);

        // create two new edges
        Edge* new_e1 = new Edge(v1, new_vert);
        Edge* new_e2 = new Edge(v2, new_vert);

        uniq_new_edges.push_back(new_e1);
        uniq_new_edges.push_back(new_e2);

        v1->add_edge(new_e1);
        v2->add_edge(new_e2);

        new_vert->add_edge(new_e1);
        new_vert->add_edge(new_e2);
    }

    // now each copy of the original vertex has 3 new edges,
    // the other end of each edge is the middle point which together consititutes a tetrahedron
    vector<Tet*> outter_tets; outter_tets.reserve(4);
    for(auto& p : vert_copies){
        Vertex* orig_vert = p.second;
        vector<Vertex*> verts_for_tet;
        for(short i = 0; i < 3; i++){
            Edge* e = orig_vert->edges[i];
            if(e->verts[0] == orig_vert) // verts[1] is the other end
                verts_for_tet.push_back(e->verts[1]);
            else // verts[0] is the other end
                verts_for_tet.push_back(e->verts[0]);
        }
        // make new tetrahedron using vertices we found
        Tet* new_tet = new Tet(orig_vert, verts_for_tet[0], verts_for_tet[1], verts_for_tet[2]);
        outter_tets.push_back(new_tet);
        orig_vert->add_tet(new_tet);
        verts_for_tet[0]->add_tet(new_tet);
        verts_for_tet[1]->add_tet(new_tet);
        verts_for_tet[2]->add_tet(new_tet);
        for(Edge* e : orig_vert->edges){ // add 3 known edges to the tetrahedron
            new_tet->add_edge(e);
        }

        // make the other 3 unknown edges which are the edges of octahedron as well
        vector<Edge*> new_edges = Utility::make_edges(verts_for_tet, true);
        for(Edge* e : new_edges){
            uniq_new_edges.push_back(e);
            octa_edges.insert(e);
            new_tet->add_edge(e);
        }
    }

    /* finding the major diagonal edge
     * steps:
     * 1. randomly pick an edge of the octahedron and randomly pick a vertex on that edge to be one side of the diagonal edge
     * 2. find the other side of the diagonal edge by using the other vertex on the randomly picked edge
    */
    vector<Tet*> inner_tets; inner_tets.reserve(4);
    Edge* diag_e = NULL;
    Vertex* first_v = uniq_edge_points[0];
    Vertex* second_v = NULL;
    // look for the octahedron edge of v2 that doesn't have connection with v1
    for(Vertex* v : uniq_edge_points){
        if(v == first_v) continue;
        if(v->is_connected_to(first_v)) continue;

        second_v = v;
        break;
    }
    diag_e = new Edge(first_v, second_v);
    first_v->add_edge(diag_e);
    second_v->add_edge(diag_e);
    uniq_new_edges.push_back(diag_e);

    /* subdivide the octahedron using diag_e
     * steps:
     * 1. find vertices of the octahedron and placed in an array which doesn't contain diagonal edge vertices
     * 2. for any two vertices in the array, check if they are connected by an edge, if yes then we form a tetrahedron by
     *      connecting these two and the the diagonal edge
    */
    // step 1
    vector<Vertex*> not_including_diag;
    for(Vertex* v : uniq_edge_points){
        if(!diag_e->has_vert(v)){
            not_including_diag.push_back(v);
        }
    }

    if(not_including_diag.size() != 4) Utility::throwErrorMessage("Tet::subdivide: not_including_diag size is not correct");

    // step 2
    for(UI i = 0; i < 4; i++){
        Vertex* vert1 = not_including_diag[i];
        for(UI j = i+1; j < 4; j++){
            Vertex* vert2 = not_including_diag[j];
            if( !vert1->is_connected_to(vert2) ) continue;

            Tet* new_tet = new Tet(diag_e->verts[0], diag_e->verts[1], vert1, vert2);
            diag_e->verts[0]->add_tet(new_tet);
            diag_e->verts[1]->add_tet(new_tet);
            vert1->add_tet(new_tet);
            vert2->add_tet(new_tet);
            inner_tets.push_back(new_tet);

            // add edges to this tetrahedron
            new_tet->add_edge(diag_e);
            diag_e->add_tet(new_tet);
            for(Edge* e : octa_edges){
                int count = 0;
                if(e->has_vert(diag_e->verts[0])) count++;
                if(e->has_vert(diag_e->verts[1])) count++;
                if(e->has_vert(vert1)) count++;
                if(e->has_vert(vert2)) count++;
                if(count == 2){
                    new_tet->add_edge(e);
                    e->add_tet(new_tet);
                }
            }
        }
    }

    // newly allocated objects need to be tracked
    for(auto& v : vert_copies){
        new_verts.push_back(v.second);
    }
    for(Vertex* v:uniq_edge_points){
        new_verts.push_back(v);
    }

    for(Edge* e : uniq_new_edges){
        new_edges.push_back(e);
    }

    for(Tet* tet : outter_tets){
        new_tets.push_back(tet);
        // create triangles, not uniquely
        tet->make_triangles();
        for(Triangle* tri : tet->tris){
            new_tris.push_back(tri);
        }
    }

    for(Tet* tet : inner_tets){
        new_tets.push_back(tet);
        // create triangles, not uniquely
        tet->make_triangles();
        for(Triangle* tri : tet->tris){
            new_tris.push_back(tri);
        }
    }
}


Eigen::Matrix3d Tet::calc_Jacobian(const Vector3d& cords, const double time)
{
    double ws[4];
    if(!this->is_pt_inside(cords, true, ws)){
        Utility::throwErrorMessage("Tet::calc_Jacobian: Error! incoming pt is outside this tet");
    }
    Eigen::Matrix3d m;
    const double twoH = h * 2;
    Vector3d px_cords = cords; px_cords.entry[0] += h;
    Vector3d nx_cords = cords; nx_cords.entry[0] -= h;
    Vector3d py_cords = cords; py_cords.entry[1] += h;
    Vector3d ny_cords = cords; ny_cords.entry[1] -= h;
    Vector3d pz_cords = cords; pz_cords.entry[2] += h;
    Vector3d nz_cords = cords; nz_cords.entry[2] -= h;

    Vertex* v_dpx, *v_dpy, * v_dpz, *v_dnx, *v_dny, *v_dnz;
    // interpolate
    v_dpx = this->get_vert_at(px_cords, time, ws, true, false);
    v_dnx = this->get_vert_at(nx_cords, time, ws, true, false);

    v_dpy = this->get_vert_at(py_cords, time, ws, true, false);
    v_dny = this->get_vert_at(ny_cords, time, ws, true, false);

    v_dpz = this->get_vert_at(pz_cords, time, ws, true, false);
    v_dnz = this->get_vert_at(nz_cords, time, ws, true, false);

    Vector3d dvx = (*v_dpx->vels[time]) - (*v_dnx->vels[time]);
    Vector3d dvy = (*v_dpy->vels[time]) - (*v_dny->vels[time]);
    Vector3d dvz = (*v_dpz->vels[time]) - (*v_dnz->vels[time]);

    dvx = dvx / twoH;

    dvy = dvy / twoH;

    dvz = dvz / twoH;

    m << dvx.x(), dvx.y(), dvx.z(),
            dvy.x(), dvy.y(), dvy.z(),
            dvz.x(), dvz.y(), dvz.z();

    delete v_dpx;
    delete v_dpy;
    delete v_dpz;
    delete v_dnx;
    delete v_dny;
    delete v_dnz;

    return m;
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
