#ifndef PRISM_TEST_HIERARCHICAL_CLUSTERING_H
#define PRISM_TEST_HIERARCHICAL_CLUSTERING_H

#include "HyperGraph.h"
#include "HierarchicalClusterer.h"
#include "test_utils.h"

pair<size_t, size_t> TestHierarchicalClustering(const string& path_to_data);

TestReport test_clusters(const vector<UndirectedGraph>& graph_clusters, HierarchicalClustererConfig config);

TestReport test_no_nodes_lost(const vector<UndirectedGraph>& graph_clusters, HyperGraph H);


#endif //PRISM_TEST_HIERARCHICAL_CLUSTERING_H
