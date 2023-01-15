#ifndef ECG_H
#define ECG_H

#include "Analysis/singularity.h"
#include "Lines/StreamLine.h"
#include <set>
#include <vector>

class ECG_EDGE;

class ECG_NODE
{
public:
    // note, a node can connect to this node in both in_edge and out_edge
    Singularity* sing; // the correspodning singularity of this node
    Vector3d cords;
    set<ECG_NODE*> in_nodes; // connected in nodes of this node
    set<ECG_NODE*> out_nodes; // connected out nodes of this node
    set<ECG_EDGE*> in_edges; // edges that into this node
    set<ECG_EDGE*> out_edges; // edges that coming out from this node

    ECG_NODE(Singularity * sing);

    UL num_inEdges() const;
    UL num_outEdges() const;
    UL num_edges() const; // should equal to num_nodes

    UL num_inNodes() const;
    UL num_outNodes() const;
    UL num_nodes() const; // should equal to num_edges, may contain duplicates

    void add_outEdge(ECG_EDGE* e);
    void add_inEdge(ECG_EDGE* e);

    void add_inNode(ECG_NODE* n);
    void add_outNode(ECG_NODE* n);

    // check if node exists in inNodes
    bool has_inNode(ECG_NODE* node) const;

    // check if node exists in outNodes
    bool has_outNode(ECG_NODE* node) const;

    // check if node exists in either in_nodes or out_nodes
    bool has_node(ECG_NODE* node) const;
};


class ECG_EDGE
{
public:
    ECG_NODE* nodes[2]; // two nodes of the Edge
    StreamLine* sl; // the streamline that corresponds to this ECG_EDGE

    ECG_EDGE();
    ECG_EDGE(ECG_NODE* first, ECG_NODE* sec);
    ECG_EDGE(ECG_NODE* first, ECG_NODE* sec, StreamLine* sl);
    ~ECG_EDGE();

    void set(ECG_NODE* first, ECG_NODE* sec);
    void set(ECG_NODE* first, ECG_NODE* sec, StreamLine* sl);

    bool has_node(ECG_NODE* node) const;
    ECG_NODE* other_node(ECG_NODE* node);
};


class ECG
{  
    // we dont allow direct access to nodes, edges and sings
private:
    // ECG nodes and singulairties are having 1-1 correspondance (nodes[0] is corresponding to sings[0])
    // using both set and vector
    set<ECG_NODE*> nodes_set;
    set<ECG_EDGE*> edges_set;
    set<Singularity*> sings_set; // uniq singularities
    vector<ECG_NODE*> nodes;
    vector<ECG_EDGE*> edges;
    vector<Singularity*> sings;

public:
    double t;
    vector<StreamLine*> sls;

    ECG(const double t);
    ~ECG(); // delete memory used for vectors

    UL num_nodes() const ;
    UL num_edges() const ;
    UL num_sings() const;
    UL num_sls() const;

    vector<Singularity*> get_sings();
    vector<ECG_NODE*> get_nodes();
    vector<ECG_EDGE*> get_edges();

    void add_sing(Singularity* );
    void add_node(ECG_NODE* );
    void add_edge(ECG_EDGE* );
    void add_sl(StreamLine*);


    vector<vector<StreamLine*>> placing_random_seeds(Mesh* mesh, UL num_of_seeds) const ;
    void build_ECG_NODES();
    void build_ECG_EDGES(Mesh* mesh, vector<vector<StreamLine*>> sls_for_all_sings);
    const vector<ECG_NODE*> get_Zero_InDegree_Nodes() const;
    const vector<ECG_NODE*> get_Zero_OutDegree_Nodes() const;
    const vector<ECG_NODE*> get_isolated_Nodes() const;

    ECG_NODE* is_close_to_node(const Vector3d& cords) const;
};
void build_ECGs(vector<Mesh*> meshes);

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

inline vector<Singularity *> ECG::get_sings()
{
    return this->sings;
}

inline vector<ECG_NODE *> ECG::get_nodes()
{
    return this->nodes;
}

inline vector<ECG_EDGE *> ECG::get_edges()
{
    return this->edges;
}

inline UL ECG_NODE::num_edges() const
{
    return this->num_inEdges() + this->num_outEdges();
}

inline unsigned long ECG_NODE::num_inNodes() const
{
    return this->in_nodes.size();
}

inline unsigned long ECG_NODE::num_outNodes() const
{
    return this->out_nodes.size();
}

inline unsigned long ECG_NODE::num_inEdges() const
{
    return this->in_edges.size();
}

inline unsigned long ECG_NODE::num_outEdges() const
{
    return this->out_edges.size();
}

inline unsigned long ECG_NODE::num_nodes() const
{
    return this->in_nodes.size() + this->out_nodes.size();
}

inline void ECG_NODE::add_inNode(ECG_NODE *n)
{
    this->in_nodes.insert(n);
}

inline void ECG_NODE::add_outNode(ECG_NODE *n)
{
    this->out_nodes.insert(n);
}

inline bool ECG_NODE::has_inNode(ECG_NODE *node) const
{
    if(this->in_nodes.find(node) == this->in_nodes.end()) return false;
    return true;
}

inline bool ECG_NODE::has_outNode(ECG_NODE *node) const
{
    if(this->out_nodes.find(node) == this->out_nodes.end()) return false;
    return true;
}

inline bool ECG_NODE::has_node(ECG_NODE *node) const
{
    if(this->has_inNode(node) || this->has_outNode(node)) return true;
    return false;
}

inline bool ECG_EDGE::has_node(ECG_NODE *node) const
{
    if(this->nodes[0] == node || this->nodes[1] == node) return true;
    return false;
}


inline ECG_NODE *ECG_EDGE::other_node(ECG_NODE *node)
{
    if(this->nodes[0] == node) return this->nodes[1];
    return this->nodes[0];
}


inline ECG_NODE::ECG_NODE(Singularity *sing)
{
    this->sing = sing;
}


inline void ECG_NODE::add_outEdge(ECG_EDGE* e)
{
    this->out_edges.insert(e);
}

inline void ECG_NODE::add_inEdge(ECG_EDGE* e)
{
    this->in_edges.insert(e);
}
#endif // ECG_H
