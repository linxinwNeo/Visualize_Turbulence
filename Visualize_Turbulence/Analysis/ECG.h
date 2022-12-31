#ifndef ECG_H
#define ECG_H

#include "Analysis/singularity.h"
#include <vector>

class ECG_EDGE {
public:
    Singularity* first;
    Singularity* sec;

    ECG_EDGE();

    void set(Singularity* first, Singularity* second);
};

class ECG
{
public:
    vector<Singularity*> nodes;
    vector<ECG_EDGE*> edges;

    ECG();
    ~ECG();

    UL num_nodes;

    void add_fixed_pt(Singularity* );
    void add_edge(ECG_EDGE* );
};

#endif // ECG_H
