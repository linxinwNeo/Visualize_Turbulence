#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

// forward class declarations
class Vertex;
class Edge;
class Face;
class Tet;

using namespace std;

// the most basic class used that stands for a node in the model
class Vertex {
public:
    // public member variables
    int idx;
    double x;
    double y;
    double z;

    vector<Edge*> edges;  // edges that has this vertex.
    vector<Face*> faces;  // Faces that has this vertex.
    vector<Tet*> tets;   // Tetrahedrons that has this vertex.

    // public member functions
    Vertex();
    Vertex(double, double, double);
    ~Vertex();

    int num_edges();
    int num_faces();
    int num_tets();

    void add_edge(Edge*);
    void add_face(Face*);
    void add_tet(Tet*);
};

#endif // VERTEX_H
