//
// Created by dominic on 29/04/22.
//

#include "pca_utils.h"

MatrixXd PCA(const MatrixXd& input_data, int k)
{
    MatrixXd standardized_data = normalise(input_data);
    cout << "Our standardized data: " << endl;
    cout << standardized_data;
    Eigen::BDCSVD <MatrixXd> svd_solver(standardized_data, ComputeThinU);
    MatrixXd Sigma = svd_solver.singularValues().asDiagonal();
    MatrixXd U = svd_solver.matrixU();
    return U.block(0,0,input_data.rows(), k)*Sigma.block(0,0,k,k);
}

MatrixXd normalise(const MatrixXd& input_data) {
    VectorXd mean_vector;
    mean_vector = input_data.rowwise().mean();
    MatrixXd standardized_data;
    VectorXd std_dev = (input_data.colwise() - mean_vector).array().pow(2).rowwise().sum().cwiseSqrt() / pow((input_data.cols()-1), 0.5);
    standardized_data =  std_dev.cwiseInverse().asDiagonal() * (input_data.colwise() - mean_vector);
    cout<<standardized_data;
    return standardized_data;
}