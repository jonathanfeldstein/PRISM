//
// Created by jonathan on 14/04/2022.
//

#include "clustering_utils.h"

double compute_theta_sym(double alpha_sym, size_t number_of_walks_ran, size_t length_of_walk){
    math::students_t_distribution t_distr(number_of_walks_ran-1);
    double t_statistic_critical_value = quantile(complement(t_distr, alpha_sym/2));
    
    return (((double)length_of_walk -1)/pow(2*number_of_walks_ran, 0.5))*t_statistic_critical_value;
}

set<NodeRandomWalkData> get_commonly_encountered_nodes(map<size_t, NodeRandomWalkData>){
    return set<NodeRandomWalkData>();
}

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_path_similarity(vector<NodeRandomWalkData> nodes_of_type,
                                                                        size_t number_of_walks_ran,
                                                                        double theta_sym,
                                                                        RandomWalkerConfig &config){
    return pair<set<size_t>, vector<set<size_t>>>();
}