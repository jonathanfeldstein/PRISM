//
// Created by jonathan on 04/05/2022.
//

#ifndef FASTER_TEST_CLUSTERING_H
#define FASTER_TEST_CLUSTERING_H

#include "clustering_utils.h"

bool TestClustering();
bool test_theta_sym();
bool test_pca(MatrixXd matrix_example, MatrixXd solution);
bool test_compute_top_paths(vector<NodeRandomWalkData> sample_path_data, size_t max_num_paths, size_t path_length, MatrixXd expected_top_paths);
bool test_two_means(MatrixXd all_points, vector<size_t> expected_cluster_labels_after_one_iteration, vector<size_t> expected_after_two_iterations);
bool test_hierarchical_two_means(MatrixXd npc, MatrixXd nfv, vector<size_t> expected_cluster_labels);
bool test_cluster_nodes_by_truncated_hitting_times(vector<NodeRandomWalkData> nodes_of_type,double threshold_hitting_time_difference,vector<size_t> expected_clustering);
bool test_cluster_nodes_by_SK_divergence(const vector<NodeRandomWalkData> &nodes_of_type, double significance_level, size_t number_of_walks, size_t max_number_of_paths, vector<size_t> expected_clustering);
bool test_cluster_nodes_by_birch(const vector<NodeRandomWalkData> &nodes, int pca_target_dimension, int max_number_of_paths, int number_of_walks, double significance_level, vector<size_t> expected_clustering);
bool test_cluster_nodes_by_path_distribution(const vector<NodeRandomWalkData> &nodes_of_type,size_t number_of_walks,size_t length_of_walks,RandomWalkerConfig &config,vector<size_t> expected_clustering);
bool test_cluster_nodes_by_path_similarity(const vector<NodeRandomWalkData> &nodes_of_type,size_t number_of_walks,size_t length_of_walks,double theta_sym,RandomWalkerConfig &config,vector<size_t> expected_clustering);

#endif //FASTER_TEST_CLUSTERING_H
