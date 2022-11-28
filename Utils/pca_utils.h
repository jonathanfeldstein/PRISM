//
// Created by dominic on 29/04/22.
//

#ifndef PRISM_PCA_UTILS_H
#define PRISM_PCA_UTILS_H

#include <Eigen/Dense>
#include <iostream>
#include "profiling_utils.h"

using namespace std;
using namespace Eigen;

MatrixXd PCA(const MatrixXd& input_data, int k);

MatrixXd normalise(const MatrixXd& input_data);

MatrixXd covariance(const MatrixXd& input_data);
#endif //PRISM_PCA_UTILS_H

