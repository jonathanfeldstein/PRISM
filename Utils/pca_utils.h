//
// Created by dominic on 29/04/22.
//

#ifndef FASTER_PCA_UTILS_H
#define FASTER_PCA_UTILS_H

#include <Eigen/Dense>
#include <iostream>
#include "profiling_utils.h"

using namespace std;
using namespace Eigen;

MatrixXd PCA(const MatrixXd& input_data, int k);

MatrixXd normalise(const MatrixXd& input_data);

MatrixXd covariance(const MatrixXd& input_data);
#endif //FASTER_PCA_UTILS_H

