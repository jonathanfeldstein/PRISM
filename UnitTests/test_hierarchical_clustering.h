//
// Created by dominic on 01/05/22.
//

#ifndef FASTER_TEST_HIERARCHICAL_CLUSTERING_H
#define FASTER_TEST_HIERARCHICAL_CLUSTERING_H

#include "HyperGraph.h"
#include "HierarchicalClusterer.h"

bool TestHierarchicalClustering();


bool test_min_cluster_size(vector<UndirectedGraph> graph_clusters, HierarchicalClustererConfig config);

bool test_max_lambda2(vector<UndirectedGraph> graph_clusters, HierarchicalClustererConfig config);

bool test_no_nodes_lost(vector<UndirectedGraph> graph_clusters, HyperGraph H);


#endif //FASTER_TEST_HIERARCHICAL_CLUSTERING_H
