//
// Created by jonathan on 21/03/2022.
//

#include "NodeClusterRandomWalkData.h"

// Function to sort the map according
// to value in a (key-value) pairs vector
vector<pair<string, double> > sort_probabilities(map<string, int>& M, size_t number_of_walks)
{

    // Declare vector of pairs
    vector<pair<string, double> > V;

    // Copy key-value pair from Map
    // to vector of pairs
    V.reserve(M.size());
    for (auto& it : M) { // TODO check that Map doesn't change
        V.emplace_back(it.first, it.second/number_of_walks); // TODO Check if correct and efficient
    }

    // Sort using comparator function
    sort(V.begin(), V.end());

    return V;
}

NodeClusterRandomWalkData::NodeClusterRandomWalkData(vector<NodeRandomWalkData> &nodes_random_walk_data) {
    this->total_count = 0;
    for(NodeRandomWalkData node:nodes_random_walk_data){
        this->node_ids.insert(node.get_node_id());
        for(pair<string, int> path_count: node.get_path_counts()){  //TODO general loop issue
            this->path_counts[path_count.first] += path_count.second;
            this->total_count += path_count.second;
        }
    }
}

NodeClusterRandomWalkData::NodeClusterRandomWalkData(NodeRandomWalkData &nodes_random_walk_data) {
    this->total_count = 0;
    this->node_ids.insert(nodes_random_walk_data.get_node_id());
    for(pair<string, int> path_count: nodes_random_walk_data.get_path_counts()){  //TODO general loop issue
        this->path_counts[path_count.first] += path_count.second;
        this->total_count += path_count.second;
    }
}

void NodeClusterRandomWalkData::merge(NodeClusterRandomWalkData other) {
    this->node_ids.merge(other.node_ids);
    this->total_count += other.total_count;
    for(auto const& path_count: other.path_counts){
        this->path_counts[path_count.first] += path_count.second;
    }
}

size_t NodeClusterRandomWalkData::number_of_nodes() {
    return node_ids.size();
}

map<string, double> NodeClusterRandomWalkData::get_top_n_path_probabilities(size_t n, size_t number_of_walks) {
    vector<pair<string, double> > sorted_path_probabilities = sort_probabilities(this->path_counts, number_of_walks);
    map<string, double> top_n_paths_probabilities;
    size_t count{0};
    for(auto path_probability:sorted_path_probabilities){
        if(count > n){
            break;
        }
        top_n_paths_probabilities.insert(path_probability);
        count++;
    }
    return top_n_paths_probabilities; // TODO
}

set<size_t> NodeClusterRandomWalkData::get_node_ids() {
    return this->node_ids;
}