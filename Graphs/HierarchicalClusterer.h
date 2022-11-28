#ifndef PRISM_HIERARCHICALCLUSTERER_H
#define PRISM_HIERARCHICALCLUSTERER_H

#include <iostream>
#include "HyperGraph.h"
#include "../Utils/profiling_utils.h"
#include <omp.h>
#include <thread>

using namespace std;

struct HierarchicalClustererConfig{
    size_t min_cluster_size = 3;
    double max_lambda2 = 0.8;
};

class HierarchicalClusterer{
private:
    size_t min_cluster_size;
    double max_lambda2;
    HyperGraph hypergraph;
    vector<UndirectedGraph> graph_clusters;
    vector<HyperGraph> hypergraph_clusters;

public:
    HierarchicalClusterer(HyperGraph hypergraph, HierarchicalClustererConfig config);
    ~HierarchicalClusterer();

    vector<HyperGraph> & run_hierarchical_clustering();
    void get_clusters(UndirectedGraph &graph);
    vector<UndirectedGraph> get_graph_clusters();
};
#endif //PRISM_HIERARCHICALCLUSTERER_H
