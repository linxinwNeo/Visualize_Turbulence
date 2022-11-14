#ifndef TET_H
#define TET_H

#include <vector>

// forward class declarations
class Vertex;
class Edge;
class Triangle;

using namespace std;

class Tet {
public:
    // member variables
    unsigned long idx;
    vector<Vertex*> verts;  // exact 4 verts that consists of this tetrahedron
    vector<Edge*> edges;    // exact 4 edges that consists of this tetrahedron
    vector<Triangle*> tris;    // exact 4 triangles that consists of this tetrahedron
    vector<Tet*> tets;      // neighbor tetrahedrons that shares faces with this, max of 4 tets

    // member functions
    Tet();
    ~Tet();

    unsigned long num_verts();
    unsigned long num_edges();
    unsigned long num_tris();
    unsigned long num_tets();

    void add_vert(Vertex*);
    void add_edge(Edge*);
    void add_triangle(Triangle*);
    void add_tet(Tet*);
};

#endif // TET_H
