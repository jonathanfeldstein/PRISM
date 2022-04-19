//
// Created by jonathan on 21/03/2022.
//

#ifndef FASTER_NODECLUSTERRANDOMWALKDATA_H
#define FASTER_NODECLUSTERRANDOMWALKDATA_H

#include <iostream>
#include <set>
#include <map>
#include <string>
#include "NodeRandomWalkData.h"

using namespace std;

class NodeClusterRandomWalkData{
private:
    set<size_t> node_ids;
    map<string, int> path_counts;
    size_t total_count;

public:
    explicit NodeClusterRandomWalkData(vector<NodeRandomWalkData> &nodes_random_walk_data);
    explicit NodeClusterRandomWalkData(NodeRandomWalkData &nodes_random_walk_data);
    ~NodeClusterRandomWalkData();
    void merge(NodeClusterRandomWalkData other);
    size_t number_of_nodes();
    map<string, double> get_top_n_path_probabilities(size_t n, size_t number_of_walks);
    set<size_t> get_node_ids();
};
#endif //FASTER_NODECLUSTERRANDOMWALKDATA_H
