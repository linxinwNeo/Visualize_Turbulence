#ifndef MESH_H
#define MESH_H

#include <vector>
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
};

#endif // MESH_H
