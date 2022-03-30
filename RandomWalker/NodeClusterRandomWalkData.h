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
    set<string> node_names;
    map<string, int> path_counts;
    size_t total_count;

public:
    explicit NodeClusterRandomWalkData(vector<NodeRandomWalkData> &nodes_random_walk_data);
    ~NodeClusterRandomWalkData();
    void merge(NodeClusterRandomWalkData other);
    size_t number_of_nodes();
    vector<pair<string, double> > get_top_n_path_probabilities(size_t n, size_t number_of_walks);
};
#endif //FASTER_NODECLUSTERRANDOMWALKDATA_H
