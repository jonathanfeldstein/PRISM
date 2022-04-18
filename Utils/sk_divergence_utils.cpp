//
// Created by jonathan on 18/04/2022.
//

#include "sk_divergence_utils.h"

double kl_divergence(map<string, double> p, map<string, double> q){
    double total_kl_divergence{0};
    set<string> paths_p = get_keys(p);
    set<string> paths_q = get_keys(q);
    for(auto &path_p:paths_p){
        if(has(paths_q, path_p) && p[path_p]!=0){
            total_kl_divergence+=p[path_p]* log(p[path_p]/q[path_p]);
        }
    }
    return total_kl_divergence;
}

map<string, double> compute_average_distribution(map<string, double> p, map<string, double> q){
    map<string, double> average_distribution;
    for(auto &path:p){
        average_distribution[path.first] = 0.5 * path.second;
    }
    for(auto &path:q){
        if(!average_distribution.count(path.first)){
            average_distribution[path.first] += 0.5 * path.second;
        } else{
            average_distribution[path.first] = 0.5 * path.second;
        }
    }
    return average_distribution;
}

double sk_divergence(map<string, double> &p, map<string, double> &q, map<string, double> &average_distribution){
    if(average_distribution.empty()){
        average_distribution = compute_average_distribution(p, q);
    }
    return 0.5 * kl_divergence(p, average_distribution) + 0.5 * kl_divergence(q, average_distribution);
}

double compute_threshold_sk_divergence(){

}

double compute_sk_divergence_of_top_n_paths(NodeClusterRandomWalkData node_cluster_1,
                                            NodeClusterRandomWalkData node_cluster_2,
                                            size_t number_of_top_paths,
                                            size_t number_of_walks,
                                            double significance_level){

}