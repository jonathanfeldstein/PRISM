//
// Created by jonathan on 14/04/2022.
//

#ifndef FASTER_CLUSTERING_UTILS_H
#define FASTER_CLUSTERING_UTILS_H
#include "NodeClusterRandomWalkData.h"
#include "RandomWalker.h"
#include <boost/math/distributions/students_t.hpp>

using namespace std;
using namespace boost;

double compute_theta_sym(double alpha_sym, size_t number_of_walks_ran, size_t length_of_walk);
set<NodeRandomWalkData> get_commonly_encountered_nodes(map<size_t, NodeRandomWalkData> nodes_random_walk_data, size_t number_of_walks_ran, double epsilon);
pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_path_similarity(vector<NodeRandomWalkData> nodes_of_type,
size_t number_of_walks_ran,
double theta_sym,
        RandomWalkerConfig &config);

#endif //FASTER_CLUSTERING_UTILS_H
