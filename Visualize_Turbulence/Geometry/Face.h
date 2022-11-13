#ifndef FACE_H
#define FACE_H

#include <vector>

// forward class declarations
class Vertex;
class Tet;
class Edge;

using namespace std;

class Face {
public:
    // member variables
    unsigned long idx;
    vector<Vertex*> verts;      // a face can have 3/4/5... number of vertices, depending on the element type
    vector<Edge*> edges;        // edges that made of this face, can be more than 3.
    vector<Tet*> tets;          // tets that uses this face, should be one/two only.

    // member functions
    Face();
    ~Face();

    unsigned long num_verts();
    unsigned long num_edges();
    unsigned long num_tets();

    void add_vert(Vertex*);
    void add_edge(Edge*);
    void add_tets(Tet*);
};

#endif // FACE_H
