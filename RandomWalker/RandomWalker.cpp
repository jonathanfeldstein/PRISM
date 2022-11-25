//
// Created by jonathan on 21/03/2022.
//
#include "map_utils.h"
#include "RandomWalker.h"


RandomWalker::RandomWalker(HyperGraph hypergraph, RandomWalkerConfig config) {
    this->hypergraph = hypergraph;
    this->num_top_paths_for_clustering = config.num_top_paths_for_clustering;
    this->max_random_walk_length = config.max_random_walk_length;
    this->epsilon =  config.epsilon;
    this->fraction_of_max_walks_to_always_complete = 0.25;
    this->length_of_walk = this->get_length_of_random_walks();
    this->number_of_walks_for_truncated_hitting_times = this->get_number_of_walks_for_truncated_hitting_times(this->length_of_walk);
    this->number_of_predicates = hypergraph.number_of_predicates();
    this->number_of_walks_for_path_distribution = this->get_number_of_walks_for_path_distribution(this->num_top_paths_for_clustering,
                                                                                                0);
    this->max_number_of_walks = max(this->number_of_walks_for_truncated_hitting_times,
                                    this->number_of_walks_for_path_distribution);
    this->number_of_walks_ran = 0;
}

RandomWalker::~RandomWalker() {

}

size_t RandomWalker::get_length_of_random_walks() {
    size_t walk_length{0};
    if(this->hypergraph.get_estimated_graph_diameter() != 0){
        // +1 accounts for the estimation of the diameter being a lower bound of the actual diameter
        walk_length = min(this->hypergraph.get_estimated_graph_diameter() +1, this->max_random_walk_length);
    }else{
        walk_length = this->max_random_walk_length;
    }
    return walk_length;
}

size_t RandomWalker::get_number_of_walks_for_truncated_hitting_times(size_t walk_length) {
    return round(pow((length_of_walk - 1),2) / (4 * pow(this->epsilon,2)));
}

size_t RandomWalker::get_number_of_walks_for_path_distribution(size_t num_top_paths_for_clustering,
                                                               size_t number_of_unique_paths) {
    size_t max_num_of_unique_paths{0};
    if(number_of_unique_paths == 0){
        if(this->number_of_predicates > 1){
            max_num_of_unique_paths = 1 +
                    (this->number_of_predicates * (pow(this->number_of_predicates, this->length_of_walk) - 1)
                    / (this->number_of_predicates - 1.0));
        }else{
            max_num_of_unique_paths = 1 + this->max_random_walk_length;
        }
    }else{
        max_num_of_unique_paths = number_of_unique_paths;
    }
    return round(min(num_top_paths_for_clustering + 1, max_num_of_unique_paths + 1)
                    * (0.577 + log(max_num_of_unique_paths))
                    / pow(this->epsilon, 2)); //0.577 is the Euler Mascheroni constant
}

pair<map<NodeId,NodeRandomWalkData>, size_t> RandomWalker::run_random_walks(NodeId source_node) {
    map<NodeId,NodeRandomWalkData> nodes_random_walk_data;
    for(auto &node: this->hypergraph.get_nodes()){
        nodes_random_walk_data.insert({node.first, NodeRandomWalkData(node.first, node.second)});
    }
    double number_of_walks = this->max_number_of_walks * this->fraction_of_max_walks_to_always_complete;
    // run a fraction of the number of walks initially estimated
    for(size_t i{0}; i < number_of_walks; i++){
        this->update_node_data_with_random_walk(source_node, nodes_random_walk_data);
    }
    // compute a refined estimate of number of additional walks needed based on the path distribution statistics
    // obtained so far
    int number_of_additional_walks = this->compute_number_of_additional_walks(nodes_random_walk_data, number_of_walks);

    // if additional walks are needed, then run them
    if(number_of_additional_walks > 0){
        for(size_t i{0}; i < number_of_additional_walks; i++){
            this->update_node_data_with_random_walk(source_node, nodes_random_walk_data);
        }
        number_of_walks += number_of_additional_walks;
    }
    return {nodes_random_walk_data, number_of_walks};
}

void RandomWalker::update_node_data_with_random_walk(NodeId source_node,
                                                     map<NodeId, NodeRandomWalkData> &nodes_random_walk_data) {
    NodeId current_node = source_node;
    set<NodeId> encountered_nodes;
    Path path;
    for(size_t step{0}; step < this->length_of_walk; step++){
        pair<EdgeId, NodeId> next_edge_and_node = this->hypergraph.get_random_edge_and_neighbor_of_node(current_node);
        path += this->hypergraph.get_predicate(next_edge_and_node.first);
        path += ',';
        if(!has(encountered_nodes, next_edge_and_node.second)){
            nodes_random_walk_data[next_edge_and_node.second].update_number_of_hits();
            nodes_random_walk_data[next_edge_and_node.second].add_path(path);
            encountered_nodes.insert(next_edge_and_node.second);
            size_t hitting_time = step+1;
            nodes_random_walk_data[next_edge_and_node.second].update_accumulated_hitting_time(hitting_time);
        }
        current_node = next_edge_and_node.second;
    }
}

int RandomWalker::compute_number_of_additional_walks(map<NodeId, NodeRandomWalkData> &nodes_random_walk_data,
                                                     size_t number_of_completed_walks) {
    size_t number_of_unique_paths = this->compute_number_of_unique_paths(nodes_random_walk_data);

    int number_of_additional_walks_for_truncated_hitting_time =
            this->number_of_walks_for_truncated_hitting_times - number_of_completed_walks;

    int number_of_additional_walks_for_path_distribution =
            this->get_number_of_walks_for_path_distribution(this->num_top_paths_for_clustering, number_of_unique_paths)
            - number_of_completed_walks;

    int number_of_additional_walks = max(number_of_additional_walks_for_path_distribution,
                                            number_of_additional_walks_for_truncated_hitting_time);
    return number_of_additional_walks;
}

size_t RandomWalker::compute_number_of_unique_paths(map<NodeId, NodeRandomWalkData> &nodes_random_walk_data) {
    set<Path> unique_paths;
    for(NodeRandomWalkData & node: get_values(nodes_random_walk_data)){
        unique_paths.merge(get_keys(node.get_path_counts()));
    }
    return unique_paths.size();
}

map<NodeId, NodeRandomWalkData> RandomWalker::generate_node_random_walk_data(NodeId source_node) {
    pair<map<NodeId,NodeRandomWalkData>, size_t> nodes_random_walk_data_and_number_of_walks = this->run_random_walks(source_node);

    for(auto node:this->hypergraph.get_node_ids()){
        nodes_random_walk_data_and_number_of_walks
        .first[node]
        .calculate_average_hitting_time(nodes_random_walk_data_and_number_of_walks.second, this->length_of_walk);
    }
    this->number_of_walks_ran = nodes_random_walk_data_and_number_of_walks.second;
    return nodes_random_walk_data_and_number_of_walks.first;
}

size_t RandomWalker::get_number_of_walks_ran() const {
    return this->number_of_walks_ran;
}

size_t RandomWalker::get_length_of_walk() {
    return this->length_of_walk;
}
