//
// Created by jonathan on 14/04/2022.
//

#ifndef FASTER_CLUSTERING_UTILS_H
#define FASTER_CLUSTERING_UTILS_H
#include "NodeClusterRandomWalkData.h"
#include "RandomWalker.h"
#include <boost/math/distributions/students_t.hpp>
#include <Eigen/Dense>
#include "sk_divergence_utils.h"
#include "pca_utils.h"

using namespace std;
using namespace boost;
using namespace Eigen;

double compute_theta_sym(double theta_p,
                         size_t number_of_walks_ran,
                         size_t length_of_walk);

set<NodeRandomWalkData> get_commonly_encountered_nodes(const map<size_t, NodeRandomWalkData> &nodes_random_walk_data,
                                                        size_t number_of_walks_ran,
                                                        double epsilon);

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_path_similarity(vector<NodeRandomWalkData> nodes_of_type,
                                                                        size_t number_of_walks_ran,
                                                                        size_t length_of_walks,
                                                                        double theta_sym,
                                                                        RandomWalkerConfig &config);

pair<set<size_t>, vector<vector<NodeRandomWalkData>>> cluster_nodes_by_truncated_hitting_times(vector<NodeRandomWalkData> nodes_of_type,
                                                                                                double threshold_hitting_time_difference);

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_path_distribution(const vector<NodeRandomWalkData> &nodes_of_type,
                                                                          size_t number_of_walks,
                                                                          size_t length_of_walks,
                                                                          RandomWalkerConfig &config);

MatrixXd compute_top_paths(const vector<NodeRandomWalkData> &nodes_of_type,
                            size_t max_number_of_paths,
                            size_t path_length);

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_sk_divergence(const vector<NodeRandomWalkData> &nodes_of_type,
                                                                      double significance_level,
                                                                      size_t number_of_walks,
                                                                      size_t max_number_of_paths);

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_birch(const vector<NodeRandomWalkData> &nodes,
                                                              int pca_target_dimension,
                                                              int max_number_of_paths,
                                                              int number_of_walks,
                                                              float significance_level);

vector<size_t> compute_optimal_birch_clustering(MatrixXd node_path_counts,
                                                int pca_target_dimension,
                                                int number_of_walks,
                                                float significance_level);

MatrixXd compute_principal_components(MatrixXd feature_vectors,
                                      int target_dimension);

pair<set<size_t>, vector<set<size_t>>> group_nodes_by_clustering_labels(const vector<NodeRandomWalkData> &nodes,
                                                                        vector<size_t> cluster_labels);

#endif //FASTER_CLUSTERING_UTILS_H
