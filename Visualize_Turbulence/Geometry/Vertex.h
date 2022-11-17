#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
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
    Vector3d v; // velocity vector


    vector<Edge*> edges;  // edges that has this vertex.
    vector<Triangle*> faces;  // Faces that has this vertex.
    vector<Tet*> tets;   // Tetrahedrons that has this vertex.

    // public member functions
    inline Vertex();
    inline Vertex( const double, const double, const double, const double, const double, const double );
    inline Vertex( const Point );
    inline Vertex( const Point* );
    inline Vertex( const Vector3d );
    inline Vertex( const Vector3d* );
    inline Vertex( const Point, const Vector3d );
    inline Vertex( const Point*, const Vector3d* );
    inline ~Vertex();

    inline unsigned long num_edges() const;
    inline unsigned long num_faces() const;
    inline unsigned long num_tets() const;

    inline void add_edge(Edge*);
    inline void add_face(Triangle*);
    inline void add_tet(Tet*);

    Vertex* clone() const;

    inline void set_cords(const double, const double, const double);
    inline void set_v(const double, const double, const double);
    inline double x() const;
    inline double y() const;
    inline double z() const;
};


// constructors
inline Vertex::Vertex()
{
    this->idx = 0;
}


inline Vertex::Vertex( const double x, const double y, const double z,  const double vx = 0, const double vy = 0, const double vz = 0)
{
    this->idx = 0;
    this->cords.set_xyz(x,y,z);
    this->set_v(vx, vy, vz);
}


inline Vertex::Vertex( const Point p )
{
    this->cords = p;
}


inline Vertex::Vertex( const Point* p )
{
    this->cords.set_xyz(p->x, p->y, p->z);
}


inline Vertex::Vertex( const Vector3d vect )
{
    this->v = vect;
}


inline Vertex::Vertex( const Vector3d* vect )
{
    this->v.set(vect->entry[0], vect->entry[1], vect->entry[2]);
}


inline Vertex::Vertex( const Point p, const Vector3d vect )
{
    this->cords = p;
    this->v = vect;
}


inline Vertex::Vertex( const Point* p, const Vector3d* vect )
{
    this->cords.set_xyz(p->x, p->y, p->z);
    this->v.set(vect->entry[0], vect->entry[1], vect->entry[2]);
}


// destructor
Vertex::~Vertex()
{
    this->edges.clear();
    this->faces.clear();
    this->edges.clear();
}


inline unsigned long Vertex::num_edges() const
{
    return this->edges.size();
}


inline unsigned long Vertex::num_faces() const
{
    return this->faces.size();
}


inline unsigned long Vertex::num_tets() const
{
    return this->tets.size();
}


inline void Vertex::add_edge(Edge* e){
    this->edges.push_back(e);
}


inline void Vertex::add_face(Triangle* f){
    this->faces.push_back(f);
}


inline void Vertex::add_tet(Tet* tet){
    this->tets.push_back(tet);
}


inline void Vertex::set_cords(const double x, const double y, const double z)
{
    this->cords.set_xyz(x, y, z);
}


inline void Vertex::set_v(const double vx, const double vy, const double vz)
{
    this->v.set(vx, vy, vz);
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
#endif // VERTEX_H
