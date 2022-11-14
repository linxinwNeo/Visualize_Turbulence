#ifndef MESH_H
#define MESH_H

#include <vector>
#include <QString>
#include <QDebug>
#include "predefined.h"
#include "utility_functions.h"
#include "Geometry/Vertex.h"
#include "Geometry/Edge.h"
#include "Geometry/Triangle.h"
#include "Geometry/Tet.h"
#include "Geometry/Vector3d.h"



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
    vector<Triangle*> tris;
    vector<Tet*> tets;
    Vector3d rot_center;
    double radius;
    // member functions
    Mesh();
    ~Mesh();

    unsigned long num_verts();
    unsigned long num_edges();
    unsigned long num_tris();
    unsigned long num_tets();

    void add_vert(Vertex*);
    void add_edge(Edge*);
    void add_triangle(Triangle*);
    void add_tet(Tet*);

    void calc_Bounding_Sphere();
};

#endif // MESH_H
