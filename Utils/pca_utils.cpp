//
// Created by dominic on 29/04/22.
//

#include "pca_utils.h"

#include <Spectra/SymEigsSolver.h>

MatrixXd PCA(const MatrixXd& input_data, int k)
{
    MatrixXd standardized_data = normalise(input_data);
    Eigen::BDCSVD <MatrixXd> svd_solver(standardized_data, ComputeThinU);
    MatrixXd Sigma = svd_solver.singularValues().asDiagonal();
    MatrixXd U = svd_solver.matrixU();
    return U.block(0,0,input_data.rows(), k)*Sigma.block(0,0,k,k);
}

MatrixXd normalise(const MatrixXd& input_data) {
    VectorXd mean_vector;
    mean_vector = input_data.colwise().mean();
    MatrixXd mean_cleaned_data = (input_data.rowwise() - mean_vector.transpose());
    MatrixXd standardized_data;
    VectorXd std_dev = mean_cleaned_data.array().pow(2).colwise().sum().cwiseSqrt() / pow((input_data.rows()), 0.5);
    for (int i=0; i<std_dev.rows(); i++) {
        if (std_dev[i] == 0) {
            std_dev[i] = 1;
        }
    }
    standardized_data =  (mean_cleaned_data) * std_dev.cwiseInverse().asDiagonal();
    return standardized_data;
}

