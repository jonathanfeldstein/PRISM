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
using NodeId = size_t;
using EdgeId = size_t;
using Path = string;

struct RandomWalkerConfig{
    // The desired fractional uncertainty. Smaller values - longer compute times. Recommended 0.05.
    double epsilon{0.05};
    // The significance level for clustering. Smaller values - clusters favoured over single nodes. Recommended 0.01.
    double alpha{0.01};
    // The number of distinct paths used for clustering. Smaller values - shorter compute time. Recommended 3.
    size_t num_top_paths_for_clustering{3};
    // Maximum random walk length. Random walks are shorter for hypergraphs with diameter < max_random_walk_length. Recommended 5.
    size_t max_random_walk_length{5};
    // The PCA dimension used for clustering by path count distribution. Recommended 2.
    size_t pca_dim{2};
    size_t clustering_method_threshold;


    bool multiprocessing = true;

    void check_config(){
        if(epsilon <= 0 || epsilon>=1){
            throw RW_InvalidArgumentException("epsilon");
        }else if(alpha<=0 || alpha>=1){
            throw RW_InvalidArgumentException("alpha");
        }else if(pca_dim <2){
            throw RW_InvalidArgumentException("pca_dim");
        }else if(max_random_walk_length<3){
            throw RW_InvalidArgumentException("max_random_walk_length");
        }else if(num_top_paths_for_clustering<3){
            throw RW_InvalidArgumentException("num_top_paths_for_clustering");
        }
        //TODO add clustering_method_threshold and TEST
    }
};

class RandomWalker{
private:
    HyperGraph hypergraph;
    int num_top_paths_for_clustering{};
    size_t max_random_walk_length{};
    double epsilon{};
    double fraction_of_max_walks_to_always_complete{};
    int number_of_predicates{};
    int length_of_walk{};
    int max_number_of_walks{};

    pair<map<NodeId,NodeRandomWalkData>, size_t> run_random_walks(size_t source_node);
    size_t get_length_of_random_walks();
    size_t get_number_of_walks_for_truncated_hitting_times(size_t walk_length);
    size_t get_number_of_walks_for_path_distribution(size_t num_top_paths_for_clustering, size_t number_of_unique_paths);
    int compute_number_of_additional_walks(map<NodeId, NodeRandomWalkData> &nodes_random_walk_data, size_t number_of_completed_walks);
    size_t compute_number_of_unique_paths(map<NodeId, NodeRandomWalkData> &nodes_random_walk_data);

public:
    RandomWalker(HyperGraph hypergraph, RandomWalkerConfig config);
    ~RandomWalker();

    map<NodeId, NodeRandomWalkData> generate_node_random_walk_data(NodeId source_node);
    void update_node_data_with_random_walk(NodeId source_node, map<NodeId, NodeRandomWalkData> &nodes_random_walk_data);
    size_t get_number_of_walks_ran() const;
    size_t get_length_of_walk();

    int number_of_walks_for_truncated_hitting_times{};
    int number_of_walks_for_path_distribution{};
    int number_of_walks_ran{};

};
#endif //FASTER_RANDOMWALKER_H
