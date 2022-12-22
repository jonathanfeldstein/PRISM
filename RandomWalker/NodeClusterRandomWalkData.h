#ifndef PRISM_NODECLUSTERRANDOMWALKDATA_H
#define PRISM_NODECLUSTERRANDOMWALKDATA_H

#include <iostream>
#include <set>
#include <map>
#include <string>
#include "NodeRandomWalkData.h"

using namespace std;
using NodeId = size_t;
using Path = string;
using Count = size_t ;
using RandomWalkCluster = vector<NodeRandomWalkData>;

class NodeClusterRandomWalkData{
private:
    set<NodeId> node_ids;
    map<Path, int> path_counts;
    Count total_count;

public:
    explicit NodeClusterRandomWalkData(RandomWalkCluster &nodes_random_walk_data);
    explicit NodeClusterRandomWalkData(NodeRandomWalkData &nodes_random_walk_data);
    ~NodeClusterRandomWalkData();
    void merge(NodeClusterRandomWalkData other);
    size_t number_of_nodes();
    map<Path, double> get_top_n_path_probabilities(size_t n, size_t number_of_walks);
    set<NodeId> get_node_ids();
};
#endif //PRISM_NODECLUSTERRANDOMWALKDATA_H
