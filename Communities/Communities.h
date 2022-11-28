#ifndef PRISM_COMMUNITIES_H
#define PRISM_COMMUNITIES_H

#include "HyperGraph.h"
#include "RandomWalker.h"
#include "Community.h"
#include "map_utils.h"
#include "clustering_utils.h"
#include <omp.h>
#include "../Utils/profiling_utils.h"
#include "Relation.h"

using namespace std;
using NodeId = size_t;
using Cluster = set<NodeId>;

class Communities{
private:
    HyperGraph hypergraph;
    RandomWalker random_walker;
    map<size_t, Community> communities;

public:
    Communities(HyperGraph hypergraph, RandomWalkerConfig config);
    ~Communities() = default;
    size_t size();
    HyperGraph &get_hypergraph();
    void print();
    map<NodeId, Community> get_communities();
    Community get_community(NodeId source_node, RandomWalkerConfig config);

};
#endif //PRISM_COMMUNITIES_H
