#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <QString>
#include <unordered_map>

#include "Others/Point.h"
#include "Others/Vector3d.h"

// forward class declarations
class Edge;
class Triangle;
class Tet;

using namespace std;

// the most basic class used that stands for a node in the model
class Vertex {
public:
    // public member variables
    unsigned long idx;
    Point cords;

    // velocity vectors
    unordered_map<double, Vector3d*> vels; // <time, velocity>
    // vorticity vectors
    unordered_map<double, Vector3d*> vors; // <time, velocity>
    // Turbulent dynamic viscosity
    unordered_map<double, double> mus; // <time, dynamic viscosity>

    vector<Edge*> edges;  // edges that has this vertex.
    vector<Triangle*> faces;  // Faces that has this vertex.
    vector<Tet*> tets;   // Tetrahedrons that has this vertex.

    // public member functions
    inline Vertex();
    inline Vertex( const Point );
    inline Vertex( const Point* );
    inline Vertex( const double x, const double y, const double z );
    ~Vertex();

    inline Vector3d cordsVect() const ;

    inline unsigned long num_edges() const;
    inline unsigned long num_tris() const;
    inline unsigned long num_tets() const;
    inline unsigned long num_vels() const;
    inline unsigned long num_vors() const;
    inline unsigned long num_mus() const;

    inline void add_edge(Edge*);
    inline void add_triangle(Triangle*);
    inline void add_tet(Tet*);

    inline void set_cords(const double x, const double y, const double z);
    void set_vel(const double time, Vector3d* vel_ptr);
    void set_vel(const double time, const double vx, const double vy, const double vz);

    void set_vor(const double time, Vector3d* vor_ptr);
    void set_vor(const double time, const double x, const double, const double);

    void set_mu(const double time, const double mu);

    inline double x() const;
    inline double y() const;
    inline double z() const;
    inline bool has_vel_at_t(const double time) const;
    inline bool has_vor_at_t(const double time) const;
    inline bool has_mu_at_t(const double time) const;
    Vector3d linear_interpolate_vel(const int t1, const int t2, const double target_t);
    Vector3d linear_interpolate_vor(const int t1, const int t2, const double target_t);
    double linear_interpolate_mu(const int t1, const int t2, const double target_t);

    inline QString cords_str() const;
    QString vel_str(const double time) const;
    QString vor_str(const double time) const;

};


// constructors
inline Vertex::Vertex()
{
    this->idx = 0;
}


inline Vertex::Vertex( const Point p )
{
    this->cords = p;
}


inline Vertex::Vertex( const Point* p )
{
    this->cords.set_cords(p->x, p->y, p->z);
}


inline  Vertex::Vertex( const double x, const double y, const double z ){
    this->set_cords(x, y, z);
}


inline Vector3d Vertex::cordsVect() const
{
    return Vector3d(this->x(), this->y(), this->z());
}


inline unsigned long Vertex::num_edges() const
{
    return this->edges.size();
}


inline unsigned long Vertex::num_tris() const
{
    return this->faces.size();
}


inline unsigned long Vertex::num_tets() const
{
    return this->tets.size();
}


inline unsigned long Vertex::num_vels() const
{
    return this->vels.size();
}


inline unsigned long Vertex::num_vors() const
{
    return this->vors.size();
}


inline unsigned long Vertex::num_mus() const
{
    return this->mus.size();
}


inline void Vertex::add_edge(Edge* e){
    this->edges.push_back(e);
}


inline void Vertex::add_triangle(Triangle* f){
    this->faces.push_back(f);
}


inline void Vertex::add_tet(Tet* tet){
    this->tets.push_back(tet);
}


inline void Vertex::set_cords(const double x, const double y, const double z)
{
    this->cords.set_cords(x, y, z);
}


// return false if vel at time t does not exist
inline bool Vertex::has_vel_at_t(const double time) const
{
    if(vels.find(time) == vels.end()) return false;
    return true;
}


// return false if vor at time t does not exist
inline bool Vertex::has_vor_at_t(const double time) const
{
    if(vors.find(time) == vors.end()) return false;
    return true;
}


// return false if mu at time t does not exist
inline bool Vertex::has_mu_at_t(const double time) const
{
    if(mus.find(time) == mus.end()) return false;
    return true;
}


inline double Vertex::x() const
{
    return this->cords.x;
}


inline double Vertex::y() const
{
    return this->cords.y;
}


inline double Vertex::z() const
{
    return this->cords.z;
}


inline QString Vertex::cords_str() const
{
    QString str = QString( "%1, %2, %3" ).arg(this->x()).arg(this->y()).arg(this->z());
    return str;
}

#endif // VERTEX_H
