//
// Created by dominic on 29/04/22.
//

#include "pca_utils.h"

MatrixXd PCA(const MatrixXd& input_data, int k)
{
    Eigen::BDCSVD <MatrixXd> svd_solver(input_data, ComputeThinU);
    MatrixXd Sigma = svd_solver.singularValues().asDiagonal();
    MatrixXd U = svd_solver.matrixU();
    return U.block(0,0,U.rows(), k)*Sigma.block(0,0,k,k);
}