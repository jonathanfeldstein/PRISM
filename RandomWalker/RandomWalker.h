//
// Created by jonathan on 21/03/2022.
//

#ifndef FASTER_RANDOMWALKER_H
#define FASTER_RANDOMWALKER_H

#include <iostream>
#include <cmath>
#include <map>
#include "HyperGraph.h"
#include "NodeRandomWalkData.h"
#include "map_utils.h"
#include "set_utils.h"


using namespace std;

struct RandomWalkerConfig{
    double epsilon;
    size_t max_num_paths;
    size_t pca_dim;
    size_t clustering_method_threshold;
    size_t max_path_length;
    double theta_p;
    bool multiprocessing; // TODO
};

class RandomWalker{
private:
    HyperGraph hypergraph;
    int max_num_paths{};
    int max_path_length{};
    double epsilon{};
    double fraction_of_max_walks_to_always_complete{};
    int length_of_walk{};
    int number_of_walks_for_truncated_hitting_times{};
    int number_of_predicates{};
    int number_of_walks_for_path_distribution{};
    int max_number_of_walks{};
    int number_of_walks_ran{};
    mt19937 rng;

    size_t get_length_of_random_walks();
    size_t get_number_of_walks_for_truncated_hitting_times(size_t walk_length);
    size_t get_number_of_walks_for_path_distribution(size_t M, size_t number_of_unique_paths);
    pair<map<size_t,NodeRandomWalkData>, size_t> run_random_walks(size_t source_node);
    void update_node_data_with_random_walk(size_t source_node, map<size_t, NodeRandomWalkData> &nodes_random_walk_data);
    int compute_number_of_additional_walks(map<size_t, NodeRandomWalkData> &nodes_random_walk_data, size_t number_of_completed_walks);
    size_t compute_number_of_unique_paths(map<size_t, NodeRandomWalkData> &nodes_random_walk_data);

public:
    RandomWalker(HyperGraph hypergraph, RandomWalkerConfig config);
    ~RandomWalker();

    map<size_t, NodeRandomWalkData> generate_node_random_walk_data(size_t source_node);
    size_t get_number_of_walks_ran() const;
    size_t get_length_of_walk();

};
#endif //FASTER_RANDOMWALKER_H
