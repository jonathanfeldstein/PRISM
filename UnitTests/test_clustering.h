#ifndef PRISM_TEST_CLUSTERING_H
#define PRISM_TEST_CLUSTERING_H

#include "clustering_utils.h"
#include "test_utils.h"
#include "eigen_utils.h"

bool TestClustering();
TestCount test_theta_sym();
TestCount test_pca(MatrixXd matrix_example, const MatrixXd& solution);
TestCount test_compute_top_paths(const RandomWalkCluster& sample_path_data,
                                 size_t max_num_paths,
                                 size_t path_length,
                                 const MatrixXd& expected_top_paths);

TestCount test_two_means(MatrixXd all_points,
                         const vector<size_t>& expected_cluster_labels_after_one_iteration,
                         const vector<size_t>& expected_after_two_iterations);

TestCount test_hierarchical_two_means(MatrixXd npc,
                                      MatrixXd nfv,
                                      const vector<size_t>& expected_cluster_labels);

TestCount test_cluster_nodes_by_truncated_hitting_times(const RandomWalkCluster& nodes_of_type,
                                                   double threshold_hitting_time_difference,
                                                   vector<size_t> expected_clustering);

TestCount test_cluster_nodes_by_SK_divergence(const RandomWalkCluster &nodes_of_type,
                                         double significance_level,
                                         size_t number_of_walks,
                                         size_t max_number_of_paths,
                                         vector<size_t> expected_clustering);

TestCount test_cluster_nodes_by_birch(const RandomWalkCluster &nodes,
                                 int pca_target_dimension,
                                 int max_number_of_paths,
                                 int number_of_walks,
                                 double significance_level,
                                 vector<size_t> expected_clustering);

TestCount test_cluster_nodes_by_path_distribution(const RandomWalkCluster &nodes_of_type,
                                             size_t number_of_walks,
                                             size_t length_of_walks,
                                             RandomWalkerConfig &config,
                                             vector<size_t> expected_clustering);

TestCount test_cluster_nodes_by_path_similarity(const RandomWalkCluster &nodes_of_type,
                                           size_t number_of_walks,
                                           size_t length_of_walks,
                                           double theta_sym,
                                           RandomWalkerConfig &config,
                                           vector<size_t> expected_clustering);

#endif //PRISM_TEST_CLUSTERING_H
