#ifndef PRISM_CLUSTERING_UTILS_H
#define PRISM_CLUSTERING_UTILS_H
#include "NodeClusterRandomWalkData.h"
#include "RandomWalker.h"
#include <boost/math/distributions/students_t.hpp>
#include <Eigen/Dense>
#include "sk_divergence_utils.h"
#include "pca_utils.h"
#include "Relation.h"
#include "Partition.h"

using namespace std;
using namespace boost;
using namespace Eigen;
using NodeId = size_t;
using Cluster = set<NodeId>;
using Path = string;
using RandomWalkCluster = vector<NodeRandomWalkData>;

double compute_theta_sym(double theta_p,
                         size_t number_of_walks_ran,
                         size_t length_of_walk);

set<NodeRandomWalkData> get_commonly_encountered_nodes(const map<NodeId, NodeRandomWalkData> &nodes_random_walk_data,
                                                        size_t number_of_walks_ran,
                                                        double epsilon);

NodePartition cluster_nodes_by_path_similarity(RandomWalkCluster nodes_of_type,
                                               size_t number_of_walks_ran,
                                               size_t length_of_walks,
                                               double theta_sym,
                                               RandomWalkerConfig &config);

RandomWalkNodePartition cluster_nodes_by_truncated_hitting_times(RandomWalkCluster nodes_of_type,
                                                                 double threshold_hitting_time_difference);

NodePartition cluster_nodes_by_path_distribution(const RandomWalkCluster &nodes_of_type,
                                                                          size_t number_of_walks,
                                                                          size_t length_of_walks,
                                                                          RandomWalkerConfig &config);

MatrixXd compute_top_paths(const RandomWalkCluster &nodes_of_type,
                            size_t max_number_of_paths,
                            size_t path_length);

NodePartition cluster_nodes_by_sk_divergence(RandomWalkCluster &nodes_of_type,
                                                                      double significance_level,
                                                                      size_t number_of_walks,
                                                                      size_t max_number_of_paths);

NodePartition cluster_nodes_by_birch(const RandomWalkCluster &nodes,
                                                              int pca_target_dimension,
                                                              int max_number_of_paths,
                                                              int number_of_walks,
                                                              double significance_level);

vector<size_t> compute_optimal_birch_clustering(MatrixXd node_path_counts,
                                                int pca_target_dimension,
                                                int number_of_walks,
                                                float significance_level);

size_t two_means(vector<size_t> &cluster_labels,
                 MatrixXd &all_points,
                 int max_iterations,
                 double threshold,
                 size_t cluster_label_to_split);

vector<size_t> hierarchical_two_means(MatrixXd node_path_counts,
                                      MatrixXd &node_feature_vectors,
                                      int max_iterations,
                                      double convergence_threshold,
                                      int number_of_walks,
                                      double significance_level);

MatrixXd compute_principal_components(MatrixXd &feature_vectors,
                                      int target_dimension);

NodePartition group_nodes_by_clustering_labels(const RandomWalkCluster &nodes,
                                               vector<size_t> cluster_labels);

vector<size_t> standardize_cluster_labels(const vector<size_t>& cluster_labels);

#endif //PRISM_CLUSTERING_UTILS_H
