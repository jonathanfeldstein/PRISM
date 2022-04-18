//
// Created by jonathan on 18/04/2022.
//

#ifndef FASTER_HYPOTHESIS_TESTING_H
#define FASTER_HYPOTHESIS_TESTING_H

#include <Eigen/Dense>
bool hypothesis_test_path_symmetric_nodes(Eigen::MatrixXd node_path_counts, size_t number_of_walks, double theta_p);

#endif //FASTER_HYPOTHESIS_TESTING_H
