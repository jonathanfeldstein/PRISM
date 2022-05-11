//
// Created by jonathan on 21/03/2022.
//

#include "NodeRandomWalkData.h"

#include <utility>


NodeRandomWalkData::NodeRandomWalkData() = default;

NodeRandomWalkData::NodeRandomWalkData(NodeId node_id, NodeType node_type) {
    this->node_id = node_id;
    this->node_type = std::move(node_type);

}

//TODO: remove
NodeRandomWalkData::NodeRandomWalkData(NodeId node_id, NodeType node_type, double hitting_time) {
    this->node_id = node_id;
    this->node_type = std::move(node_type);
    this->average_hitting_time = hitting_time;

}

NodeRandomWalkData::~NodeRandomWalkData() =default;

bool NodeRandomWalkData::operator<(const NodeRandomWalkData &other) const{
    if (get_average_hitting_time() == other.get_average_hitting_time()){
        return this->get_node_id() < other.get_node_id();
    }else{
        return get_average_hitting_time() < other.get_average_hitting_time();
    }
}

double NodeRandomWalkData::get_average_hitting_time() const {
    return this->average_hitting_time;
}

void NodeRandomWalkData::add_path(Path const& path) {
    this->path_counts[path]++;
}

void NodeRandomWalkData::update_number_of_hits() {
    this->number_of_hits ++;
}

void NodeRandomWalkData::update_accumulated_hitting_time(size_t hitting_time) {
    this->accumulated_hitting_time += hitting_time;
}

void NodeRandomWalkData::calculate_average_hitting_time(size_t number_of_walks, size_t max_length) {
    average_hitting_time = ((double)accumulated_hitting_time + ((double)number_of_walks - number_of_hits) * max_length)/number_of_walks;
}

map<Path, int> NodeRandomWalkData::get_path_counts() {
    return this->path_counts;
}

NodeId NodeRandomWalkData::get_node_id() const {
    return this->node_id;
}

NodeType NodeRandomWalkData::get_node_type() {
    return this->node_type;
}

size_t NodeRandomWalkData::get_count_of_nth_path(size_t n) {
    vector<pair<string, int> > paths = sort(this->path_counts);
    size_t count;
    if(n < paths.size()){
        count = paths[n-1].second;
    }else if(paths.size()>0){
        count = paths.back().second;
    }else{
        count = 0;
    }
    return count;
}

// TODO clean up logic
vector<pair<Path, int> > NodeRandomWalkData::get_top_paths(size_t number_of_paths, size_t path_length) {
    vector<pair<string, int> > paths = sort(this->path_counts);
    vector<pair<string, int> > top_paths;
    vector<pair<string, int> > paths_of_path_length;
    if(path_length != 0){ // Used for Hypothesis Testing and SK divergence where we only want paths of a specific path length
        for(auto path_pair: paths) {
            if (count(path_pair.first.begin(), path_pair.first.end(),',') == path_length-1) { // e.g. a path of length 3 has two commas
                paths_of_path_length.emplace_back(path_pair);
            }
        }
    }
    if(number_of_paths < path_counts.size()){
        if(path_length != 0){
            top_paths = vector<pair<string, int> >(paths_of_path_length.begin(),
                                                   paths_of_path_length.begin()+min(number_of_paths,paths_of_path_length.size()));//[:number_of_paths]
        }else{
            top_paths = vector<pair<string, int> >(paths.begin(),
                                                   paths.begin()+number_of_paths);//[:number_of_paths]
        }

    }else{
        if(path_length !=0){
            top_paths = paths_of_path_length;
        }else{
            top_paths = paths;
        }
    }

    return top_paths;
}

size_t NodeRandomWalkData::get_number_of_hits() const {
    return this->number_of_hits;
}

