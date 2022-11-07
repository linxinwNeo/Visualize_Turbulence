#ifndef TET_H
#define TET_H

#include <vector>

// forward class declarations
class Vertex;
class Edge;
class Face;

using namespace std;

class Tet {
public:
    // member variables
    int idx;
    vector<Vertex*> verts;  // exact 4 verts that consists of this tetrahedron
    vector<Edge*> edges;    // exact 4 edges that consists of this tetrahedron
    vector<Face*> faces;    // exact 4 faces that consists of this tetrahedron
    vector<Tet*> tets;      // neighbor tetrahedrons that shares faces with this, max of 4 tets

    // member functions
    Tet();
    ~Tet();

    int num_verts();
    int num_edges();
    int num_faces();
    int num_tets();

    void add_vert(Vertex*);
    void add_edge(Edge*);
    void add_face(Face*);
    void add_tet(Tet*);
};

#endif // TET_H
