#ifndef ECG_H
#define ECG_H

#include "Analysis/singularity.h"
#include "Lines/StreamLine.h"
#include <vector>

class ECG_EDGE;

class ECG_NODE
{
public:
    Singularity* node; // the correspodning singularity of this node
    vector<ECG_NODE*> nodes; // connected nodes of this node
    vector<ECG_EDGE*> edges; // connected edges of this node

    ECG_NODE(Singularity * sing);

    UL num_edges() const; // should equal to num_nodes
    UL num_nodes() const; // should equal to num_edges

    void add_nodes(ECG_NODE* n);
    void add_edges(ECG_EDGE* e);
};


class ECG_EDGE
{
public:
    ECG_NODE* nodes[2]; // two nodes of the Edge
    StreamLine* sl; // the streamline that corresponds to this ECG_EDGE

    ECG_EDGE();
    ~ECG_EDGE();

    void set(ECG_NODE* first, ECG_NODE* sec);
    void set(ECG_NODE* first, ECG_NODE* sec, StreamLine* sl);
};


class ECG
{
public:
    // ECG nodes and singulairties are having 1-1 correspondance (nodes[0] is corresponding to sings[0])
    double t;
    vector<ECG_NODE*> nodes;
    vector<ECG_EDGE*> edges;

    vector<Singularity*> sings; // uniq singularities
    vector<StreamLine*> sls;

    ECG(const double t);
    ~ECG(); // delete memory used for vectors

    UL num_nodes() const ;
    UL num_edges() const ;
    UL num_sings() const;
    UL num_sls() const;

    void add_sing(Singularity* );
    void add_node(ECG_NODE* );
    void add_edge(ECG_EDGE* );
    void add_sl(StreamLine*);


    vector<vector<StreamLine*>> placing_random_seeds(Mesh* mesh, UL num_of_seeds) const ;
    void build_ECG_NODES();
    void build_ECG_EDGES(Mesh* mesh, vector<vector<StreamLine*>> sls_for_all_sings);
};

inline UL ECG::num_nodes() const
{
    return this->nodes.size();
}

inline UL ECG::num_sings() const
{
    return this->sings.size();
}

inline UL ECG::num_edges() const
{
    return this->edges.size();
}

inline unsigned long ECG::num_sls() const
{
    return this->sls.size();
}

inline UL ECG_NODE::num_edges() const
{
    return this->edges.size();
}

inline unsigned long ECG_NODE::num_nodes() const
{
    return this->nodes.size();
}

#endif // ECG_H
