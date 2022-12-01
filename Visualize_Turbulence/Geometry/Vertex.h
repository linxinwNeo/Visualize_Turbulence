#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <QString>
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
    vector<Vector3d*> vels; // velocity vector
    vector<Vector3d*> vors; // vorticity vector
    vector<double> mus; // Turbulent dynamic viscosity

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

    Vertex* clone() const;

    inline void set_cords(const double, const double, const double);
    inline void set_vel(const double, const double, const double);
    inline void set_vor(const double, const double, const double);
    inline double x() const;
    inline double y() const;
    inline double z() const;

    inline QString cords_str() const;
    inline QString vel_str(unsigned long) const;
    inline QString vor_str(unsigned long) const;


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


inline void Vertex::set_vel(const double vx, const double vy, const double vz)
{
    Vector3d* new_vel = new Vector3d(vx, vy, vz);
    this->vels.push_back(new_vel);
}


inline void Vertex::set_vor(const double vx, const double vy, const double vz)
{
    Vector3d* new_vor = new Vector3d(vx, vy, vz);
    this->vors.push_back(new_vor);
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


inline QString Vertex::vel_str( unsigned long idx ) const
{
    if( idx >= this->num_vels() || idx < 0 ) return "idx is invalid";
    QString str = QString( "%1, %2, %3" ).arg(this->vels[idx]->entry[0]).arg(this->vels[idx]->entry[1]).arg(this->vels[idx]->entry[2]);
    return str;
}


inline QString Vertex::vor_str( unsigned long idx ) const
{
    if( idx >= this->num_vors() || idx < 0 ) return "idx is invalid";
    QString str = QString( "%1, %2, %3" ).arg(this->vors[idx]->entry[0]).arg(this->vors[idx]->entry[1]).arg(this->vors[idx]->entry[2]);
    return str;

}
#endif // VERTEX_H
