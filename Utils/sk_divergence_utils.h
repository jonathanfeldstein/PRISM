//
// Created by jonathan on 18/04/2022.
//

#ifndef FASTER_SK_DIVERGENCE_UTILS_H
#define FASTER_SK_DIVERGENCE_UTILS_H
#include <iostream>
#include <vector>
#include "NodeClusterRandomWalkData.h"
#include "map_utils.h"
#include "set_utils.h"
#include "stats_utils.h"
#include <math.h>

using namespace std;


double kl_divergence(map<string, double> p, map<string, double> q);
map<string, double> compute_average_distribution(map<string, double> p, map<string, double> q);
double sk_divergence(map<string, double> &p, map<string, double> &q, map<string, double> &average_distribution);
double compute_threshold_sk_divergence(size_t number_of_walks,
                                       map<string, double> &average_paths_probabilities,
                                       size_t number_of_top_paths,
                                       double significance_level);
pair<double, double> compute_sk_divergence_of_top_n_paths(NodeClusterRandomWalkData node_cluster1,
                                                          NodeClusterRandomWalkData node_cluster2,
                                                          size_t number_of_top_paths,
                                                          size_t number_of_walks,
                                                          double significance_level);

#endif //FASTER_SK_DIVERGENCE_UTILS_H
