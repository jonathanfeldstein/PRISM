//
// Created by jonathan on 09/12/2022.
//

#ifndef PRISM_EIGEN_UTILS_H
#define PRISM_EIGEN_UTILS_H

#include <Eigen/Dense>
#include <iostream>
using namespace std;
using namespace Eigen;

template<typename T>
static string toString(const Eigen::MatrixBase<T>& M){
    stringstream ss;
    ss << M;
    return ss.str();
};

#endif //PRISM_EIGEN_UTILS_H
