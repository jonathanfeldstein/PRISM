//
// Created by jonathan on 14/04/2022.
//

#include "clustering_utils.h"
#include "hypothesis_testing.h"

double compute_theta_sym(double alpha_sym, size_t number_of_walks_ran, size_t length_of_walk){
    math::students_t_distribution t_distr(number_of_walks_ran-1);
    double t_statistic_critical_value = quantile(complement(t_distr, alpha_sym/2));

    return (((double)length_of_walk -1)/pow(2*number_of_walks_ran, 0.5))*t_statistic_critical_value;
}

set<NodeRandomWalkData> get_commonly_encountered_nodes(map<size_t, NodeRandomWalkData> nodes_random_walk_data, size_t number_of_walks_ran, double epsilon){
    set<NodeRandomWalkData> commonly_encountered_nodes;
    for(auto node : nodes_random_walk_data){
        if(node.second.get_count_of_nth_path(3) >= number_of_walks_ran/(number_of_walks_ran*pow(epsilon, 2)+1)){
            commonly_encountered_nodes.insert(node.second);
        }
    }
    return commonly_encountered_nodes;
}

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_path_similarity(vector<NodeRandomWalkData> nodes_of_type,
                                                                        size_t number_of_walks_ran,
                                                                        double theta_sym,
                                                                        RandomWalkerConfig &config){
    //    Clusters nodes from a hypergraph into groups which are symmetrically related relative to a source node.
    //
    //            Firstly, nodes are grouped into distance-symmetric clusters; sets of nodes where the difference in the average
    //    truncated hitting times from the source node for any two nodes in the set is no greater than a specified threshold.
    //
    //            Secondly, within each distance-symmetric cluster, we further cluster together nodes based on the similarity
    //    in their distribution of paths to yield a set of path-symmetric clusters. If the cluster is a singleton set then we
    //    add its member to a set of 'single nodes'.
    //
    //            returns: the set of single nodes and a list of path-symmetric node clusters
    set<size_t> single_nodes;
    vector<set<size_t>> clusters;

    pair<set<size_t>, vector<vector<NodeRandomWalkData>>> distance_symmetric_clusters = cluster_nodes_by_truncated_hitting_times(
            nodes_of_type, theta_sym);

    single_nodes.merge(distance_symmetric_clusters.first);

    for (auto distance_symmetric_cluster: distance_symmetric_clusters.second) {
        pair<set<size_t>, vector<set<size_t>>> path_symmetric_clusters = cluster_nodes_by_path_distribution(distance_symmetric_cluster,
                                                                                                            number_of_walks_ran,
                                                                                                            config);

        single_nodes.merge(path_symmetric_clusters.first);
        clusters.insert(clusters.end(), path_symmetric_clusters.second.begin(), path_symmetric_clusters.second.end());

    }

    return {single_nodes, clusters};
}

pair<set<size_t>, vector<vector<NodeRandomWalkData>>> cluster_nodes_by_truncated_hitting_times(vector<NodeRandomWalkData> nodes_of_type,
                                                                                               double threshold_hitting_time_difference) {

    // TODO check that this indeed sorts from least to biggest
    sort(nodes_of_type.begin(), nodes_of_type.end());
    double current_hitting_time = nodes_of_type[0].get_average_hitting_time();
    vector<vector<NodeRandomWalkData>> distance_symmetric_clusters;
    vector<NodeRandomWalkData> distance_symmetric_cluster;
    set<size_t> distance_symmetric_single_nodes;

    for(auto node: nodes_of_type){
        if(node.get_average_hitting_time()-current_hitting_time < threshold_hitting_time_difference){
            distance_symmetric_cluster.emplace_back(node);
        }else{
            if(distance_symmetric_cluster.size() == 1){
                distance_symmetric_single_nodes.insert(distance_symmetric_cluster[0].get_node_id());
            }else{
                distance_symmetric_clusters.emplace_back(distance_symmetric_cluster);
            }
            distance_symmetric_cluster.clear();
            distance_symmetric_cluster.emplace_back(node);
        }
        current_hitting_time = node.get_average_hitting_time();
    }

    if (!distance_symmetric_cluster.empty()) {
        if (distance_symmetric_cluster.size() > 1) {
            distance_symmetric_clusters.emplace_back(distance_symmetric_cluster);
        } else {
            distance_symmetric_single_nodes.insert(distance_symmetric_cluster[0].get_node_id());
        }
    }

    return {distance_symmetric_single_nodes, distance_symmetric_clusters};
}

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_path_distribution(vector<NodeRandomWalkData> nodes_of_type,
                                                                          size_t number_of_walks,
                                                                          RandomWalkerConfig &config){
    MatrixXd node_path_counts = compute_top_paths(nodes_of_type, config.max_num_paths);
    set<size_t> single_nodes;
    vector<set<size_t>> clusters;

    if (hypothesis_test_path_symmetric_nodes(node_path_counts, number_of_walks, config.theta_p)) {
        set<size_t> cluster;
        for(auto node:nodes_of_type){
            cluster.insert(node.get_node_id());
        }
        clusters.emplace_back(cluster);
    }else{
        if(nodes_of_type.size() <= config.clustering_method_threshold){
            pair<set<size_t>, vector<set<size_t>>> sk_clusters = cluster_nodes_by_sk_divergence(nodes_of_type,
                                                                                                config.theta_p,
                                                                                                number_of_walks,
                                                                                                config.max_num_paths);
            single_nodes = sk_clusters.first; // TODO Check for potential memory leakage
            clusters = sk_clusters.second;
        }else{
            //TODO implement birch
        }
    }
    return {single_nodes, clusters};
}

MatrixXd compute_top_paths(vector<NodeRandomWalkData> nodes_of_type, size_t max_number_of_paths) {
    vector<vector<pair<string, int>>> top_paths_of_each_node;
    for (auto node: nodes_of_type) {
        top_paths_of_each_node.emplace_back(node.get_top_paths(max_number_of_paths));
    }
    set<string> unique_paths;
    for (auto paths: top_paths_of_each_node) {
        for (auto path_pair: paths) {
            unique_paths.insert(path_pair.first);
        }
    }

    size_t number_of_unique_paths;
    number_of_unique_paths = unique_paths.size();
    unique_paths.clear();

    map<string, size_t> path_string_to_path_index;
    MatrixXd node_path_counts = MatrixXd::Zero(number_of_unique_paths, nodes_of_type.size());
    size_t node_index{0};
    for(auto node_paths: top_paths_of_each_node){
        for(auto path:node_paths){
            size_t path_index{};
            if(!has(get_keys(path_string_to_path_index), path.first)){
                path_index =path_string_to_path_index.size();
                path_string_to_path_index[path.first] = path_index;
            }else{
                path_index = path_string_to_path_index[path.first];
            }
            node_path_counts(path_index,node_index) = path.second;
        }
        node_index++;
    }

    return node_path_counts;

}

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_sk_divergence(vector<NodeRandomWalkData> nodes_of_type,
                                                                      double significance_level,
                                                                      size_t number_of_walks,
                                                                      size_t max_number_of_paths) {
    // TODO improve efficiency of emplace back
    vector<NodeClusterRandomWalkData> sk_clusters;
    for (auto node: nodes_of_type) {
        sk_clusters.emplace_back(NodeClusterRandomWalkData(node));
    }

    double max_divergence = numeric_limits<double>::infinity();

    size_t cluster_to_merge1{0};
    size_t cluster_to_merge2{0};
    while (true) {
        double smallest_divergence = max_divergence;
        for (size_t i = 0; i < sk_clusters.size() ; i++) {
            for (size_t j = i + 1; j < sk_clusters.size(); j++) {
                pair<double, double> sk_divergence_and_threshold = compute_sk_divergence_of_top_n_paths(sk_clusters[i], sk_clusters[j], max_number_of_paths, number_of_walks, significance_level);

                if (sk_divergence_and_threshold.first < smallest_divergence && sk_divergence_and_threshold.first < sk_divergence_and_threshold.second) {
                    smallest_divergence = sk_divergence_and_threshold.first;
                    cluster_to_merge1 = i;
                    cluster_to_merge2 = j;
                }
            }
        }

        if (smallest_divergence < max_divergence) {
            sk_clusters[cluster_to_merge1].merge(sk_clusters[cluster_to_merge2]);
            sk_clusters.erase(sk_clusters.begin()+cluster_to_merge2);
        } else {
            break;
        }
    }

    set<size_t> single_nodes;
    vector<set<size_t>> clusters;
    for (auto sk_cluster: sk_clusters) {
        if (sk_cluster.number_of_nodes() == 1) {
            size_t node_id = *sk_cluster.get_node_ids().begin();
            single_nodes.insert(node_id);
        } else {
            clusters.emplace_back(sk_cluster.get_node_ids());
        }
    }

    return {single_nodes, clusters};
}