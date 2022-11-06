#ifndef EDGE_H
#define EDGE_H

#include <vector>

// forward class declarations
class Vertex;
class Tet;
class Face;

using namespace std;

class Edge {
public:
    // member variables
    int idx;
    vector<Vertex*> verts;    // an edge has two vertices
    vector<Face*> faces;    // faces that contain this edge
    vector<Tet*> tets;      // tetrahedrons that contain this edge

    // member functions
    Edge();
    ~Edge();
};

#endif // EDGE_H
