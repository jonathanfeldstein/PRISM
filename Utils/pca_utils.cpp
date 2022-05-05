//
// Created by dominic on 29/04/22.
//

#include "pca_utils.h"

#include <Spectra/SymEigsSolver.h>

MatrixXd PCA(const MatrixXd& input_data, int k)
{
    Timer pca_time("PCA time");
    MatrixXd standardized_data = normalise(input_data);
//    MatrixXd cov = covariance(standardized_data);
//    Spectra::DenseSymMatProd<double> op(cov);
//
//    // Construct eigen solver object, requesting the largest three eigenvalues
//    Spectra::SymEigsSolver<Spectra::DenseSymMatProd<double>> eigs(op, k, k+1);
//    eigs.init();
//    eigs.compute(Spectra::SortRule::LargestAlge);
//    MatrixXd evectors = eigs.eigenvectors();
    VectorXd mean;
    mean =  standardized_data.colwise().mean();
    MatrixXd final;
    final = (standardized_data.transpose().colwise() - mean).transpose();
//    pca_time.Stop();
//    return  final * evectors;
    Eigen::BDCSVD <MatrixXd> svd_solver(final, ComputeThinU);
    MatrixXd Sigma = svd_solver.singularValues().asDiagonal();
    MatrixXd U = svd_solver.matrixU();
    pca_time.Stop();
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

MatrixXd covariance(const MatrixXd& input_data){
    MatrixXd centered = input_data.rowwise() - input_data.colwise().mean();
    cout << "Centered adjoint"<< endl;
    cout << centered.adjoint() << endl;
    return (centered.adjoint() * centered) / double(input_data.rows() - 1);
}
