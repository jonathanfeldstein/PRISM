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

#endif //FASTER_TEST_CLUSTERING_H
