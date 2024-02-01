#ifndef PRISM_CONCEPTS_H
#define PRISM_CONCEPTS_H

#include "../Graphs/HyperGraph.h"
#include "../RandomWalker/RandomWalker.h"
#include "Concept.h"
#include "../Utils/map_utils.h"
#include "../Utils/clustering_utils.h"
#include <omp.h>
#include "../Utils/profiling_utils.h"
#include "../Graphs/Relation.h"

using namespace std;
using NodeId = size_t;
using Cluster = set<NodeId>;

class Concepts{
private:
    HyperGraph hypergraph;
    RandomWalker random_walker;
    map<size_t, Concept> concepts;

public:
    Concepts(HyperGraph hypergraph, RandomWalkerConfig config);
    ~Concepts() = default;
    size_t size();
    HyperGraph &get_hypergraph();
    void print();
    map<NodeId, Concept> get_concepts();
    Concept get_concept(NodeId source_node, RandomWalkerConfig config);

};
#endif //PRISM_CONCEPTS_H
