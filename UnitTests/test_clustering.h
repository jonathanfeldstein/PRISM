#ifndef PRISM_TEST_CLUSTERING_H
#define PRISM_TEST_CLUSTERING_H

#include "clustering_utils.h"
#include "test_utils.h"
#include "eigen_utils.h"

pair<size_t, size_t> TestClustering();
TestReport test_theta_sym();
TestReport test_pca(MatrixXd matrix_example, const MatrixXd& solution);
TestReport test_compute_top_paths(const RandomWalkCluster& sample_path_data,
                                  size_t max_num_paths,
                                  size_t path_length,
                                  const MatrixXd& expected_top_paths);

TestReport test_two_means(MatrixXd all_points,
                          const vector<size_t>& expected_cluster_labels_after_one_iteration,
                          const vector<size_t>& expected_after_two_iterations);

TestReport test_hierarchical_two_means(MatrixXd npc,
                                       MatrixXd nfv,
                                       const vector<size_t>& expected_cluster_labels);

TestReport test_cluster_nodes_by_truncated_hitting_times(const RandomWalkCluster& nodes_of_type,
                                                         double threshold_hitting_time_difference,
                                                         const vector<size_t>& expected_clustering);

TestReport test_cluster_nodes_by_SK_divergence(RandomWalkCluster nodes_of_type,
                                               double significance_level,
                                               size_t number_of_walks,
                                               size_t max_number_of_paths,
                                               const vector<size_t>& expected_clustering);

TestReport test_cluster_nodes_by_birch(const RandomWalkCluster& nodes,
                                       int pca_target_dimension,
                                       int max_number_of_paths,
                                       int number_of_walks,
                                       double significance_level,
                                       const vector<size_t>& expected_clustering);

TestReport test_cluster_nodes_by_path_distribution(const RandomWalkCluster& nodes_of_type,
                                                   size_t number_of_walks,
                                                   size_t length_of_walks,
                                                   RandomWalkerConfig &config,
                                                   const vector<size_t>& expected_clustering);

TestReport test_cluster_nodes_by_path_similarity(const RandomWalkCluster &nodes_of_type,
                                                 size_t number_of_walks,
                                                 size_t length_of_walks,
                                                 double significance_level,
                                                 RandomWalkerConfig &config,
                                                 const vector<size_t>& expected_clustering);

#endif //PRISM_TEST_CLUSTERING_H
