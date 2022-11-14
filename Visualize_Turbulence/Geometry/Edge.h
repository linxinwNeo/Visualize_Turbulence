#ifndef EDGE_H
#define EDGE_H

#include <vector>

// forward class declarations
class Vertex;
class Tet;
class Triangle;

using namespace std;

class Edge {
public:
    // member variables
    unsigned long  idx;
    vector<Vertex*> verts;    // an edge has two vertices
    vector<Triangle*> tris;    // faces that contain this edge
    vector<Tet*> tets;      // tetrahedrons that contain this edge

    // member functions
    Edge();
    ~Edge();

    unsigned long num_verts();
    unsigned long num_tris();
    unsigned long num_tets();

    void add_vert(Vertex*);
    void add_triangle(Triangle*);
    void add_tet(Tet*);
};

#endif // EDGE_H
