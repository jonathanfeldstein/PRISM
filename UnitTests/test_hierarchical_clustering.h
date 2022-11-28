//
// Created by dominic on 01/05/22.
//

#ifndef PRISM_TEST_HIERARCHICAL_CLUSTERING_H
#define PRISM_TEST_HIERARCHICAL_CLUSTERING_H

#include "HyperGraph.h"
#include "HierarchicalClusterer.h"

bool TestHierarchicalClustering(string path_to_data);

bool test_clusters(vector<UndirectedGraph> graph_clusters, HierarchicalClustererConfig config);

bool test_no_nodes_lost(vector<UndirectedGraph> graph_clusters, HyperGraph H);


#endif //PRISM_TEST_HIERARCHICAL_CLUSTERING_H
