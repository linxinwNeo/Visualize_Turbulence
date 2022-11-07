#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QString>
#include "Geometry/Vertex.h"
#include "Geometry/Edge.h"
#include "Geometry/Face.h"
#include "Geometry/Tet.h"

// forward class declarations
class Vertex;
class Edge;
class Face;
class Tet;

using namespace std;

class Mesh {
public:
    // member variables
    vector<Vertex*> verts;
    vector<Edge*> edges;
    vector<Face*> faces;
    vector<Tet*> tets;

    // member functions
    Mesh();
    ~Mesh();

    int num_verts();
    int num_edges();
    int num_faces();
    int num_tets();

    void add_vert(Vertex*);
    void add_edge(Edge*);
    void add_face(Face*);
    void add_tet(Tet*);
};

#endif // MESH_H
