#include "NodeClusterRandomWalkData.h"


NodeClusterRandomWalkData::NodeClusterRandomWalkData(RandomWalkCluster &nodes_random_walk_data) {
    this->total_count = 0;
    for(const NodeRandomWalkData& node:nodes_random_walk_data){
        this->node_ids.insert(node.get_node_id());
        for(const auto& path_count: node.get_path_counts()){
            this->path_counts[path_count.first] += path_count.second;
            this->total_count += path_count.second;
        }
    }
}

NodeClusterRandomWalkData::NodeClusterRandomWalkData(NodeRandomWalkData &nodes_random_walk_data) {
    this->total_count = 0;
    this->node_ids.insert(nodes_random_walk_data.get_node_id());
    for(const auto& path_count: nodes_random_walk_data.get_path_counts()){
        this->path_counts[path_count.first] += path_count.second;
        this->total_count += path_count.second;
    }
}

NodeClusterRandomWalkData::~NodeClusterRandomWalkData() = default;

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

map<Path, double> NodeClusterRandomWalkData::get_top_n_path_probabilities(size_t n, size_t number_of_walks) {
    vector<pair<Path, double> > sorted_path_probabilities = normalize_and_sort_counts(this->path_counts,
                                                                                      number_of_walks);
    map<Path, double> top_n_paths_probabilities;
    size_t count{0};
    for(auto path_probability:sorted_path_probabilities){
        if(count > n){
            break;
        }
        top_n_paths_probabilities.insert(path_probability);
        count++;
    }
    return top_n_paths_probabilities;
}

set<NodeId> NodeClusterRandomWalkData::get_node_ids() {
    return this->node_ids;
}


