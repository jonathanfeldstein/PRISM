#ifndef PRISM_HYPOTHESIS_TESTING_H
#define PRISM_HYPOTHESIS_TESTING_H

#include <Eigen/Dense>
#include <iostream>
#include "stats_utils.h"
#include "../RandomWalker/NodeRandomWalkData.h"
#include "clustering_utils.h"

using namespace std;
using namespace Eigen;
using RandomWalkCluster = vector<NodeRandomWalkData>;

bool hypothesis_test_path_symmetric_nodes(RandomWalkCluster nodes_of_type,
                                          size_t number_of_walks,
                                          size_t num_top_paths_for_clustering,
                                          size_t length_of_walks,
                                          double theta_p);

bool hypothesis_test_on_node_path_counts(MatrixXd node_path_counts, size_t number_of_walks, double theta_p);

pair<double, double> covariance_matrix_sum_of_diagonals_and_sum_of_squares(size_t N, size_t P, VectorXd &c_vector);

void append_null_counts(MatrixXd &node_path_counts,
                        size_t N);

bool Q_test(double Q_critical,
            MatrixXd &c_matrix,
            VectorXd &c_vector,
            size_t V,
            size_t P);

#endif //PRISM_HYPOTHESIS_TESTING_H
