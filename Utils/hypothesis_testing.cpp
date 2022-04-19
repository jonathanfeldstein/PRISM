//
// Created by jonathan on 18/04/2022.
//

#include "hypothesis_testing.h"

bool hypothesis_test_path_symmetric_nodes(MatrixXd &node_path_counts, size_t number_of_walks, double theta_p){
    append_null_counts(node_path_counts, number_of_walks); //Appends to node_path_counts the null path counts
    size_t number_of_paths = node_path_counts.rows();
    size_t number_of_nodes = node_path_counts.cols();

    // A single node is trivially of the same probability distribution as itself
    if (number_of_nodes == 1) {
        return true;
    }

    VectorXd mean_path_counts = node_path_counts.rowwise().mean();

    // For one path, a different statistical test is required than when there is more than one path
    if (number_of_paths == 1) {
        return true;
        //TODO: implement
    } else {
        MatrixXd cov_matrix = covariance_matrix_of_count_residues(number_of_walks,
                                                                  number_of_nodes,
                                                                  number_of_paths,
                                                                  mean_path_counts);
        EigenSolver<MatrixXd> eigen_solver(cov_matrix);

        VectorXd covariance_eigenvalues = eigen_solver.eigenvalues().real();
        double Q_critical = compute_non_central_chi_squared_critical_value(covariance_eigenvalues,
                                                                           number_of_nodes,
                                                                           theta_p);

        return Q_test_if_multiple_paths(Q_critical,
                                        node_path_counts,
                                        mean_path_counts,
                                        number_of_nodes,
                                        number_of_paths);
    }

}

MatrixXd covariance_matrix_of_count_residues(size_t N, size_t V, size_t P, VectorXd &c_vector){

    MatrixXd Sigma = MatrixXd::Zero(P,P);
    for (int i{0}; i < P; i ++) {
        for (int j{0}; j < P; j ++) {
            if (i == j) {
                Sigma(i,j) = (c_vector[i] / N) * (1 - c_vector[j] / N);
            } else {
                Sigma(i,j) = - (c_vector[i] * c_vector[j]) / (pow(N, 2));
            }
        }
    }
    Sigma *= N * (1 - 1 / V);

    return Sigma;
}

void append_null_counts(MatrixXd &node_path_counts, size_t N){
    VectorXd zero_counts = (VectorXd::Ones(node_path_counts.cols()) * N) - node_path_counts.colwise().sum().transpose();
    node_path_counts.conservativeResize(node_path_counts.rows()+1, node_path_counts.cols());
    node_path_counts.row(node_path_counts.rows()-1) = zero_counts;
}
bool Q_test_if_single_path(){
    return true; // TODO
}
bool Q_test_if_multiple_paths(double Q_critical, MatrixXd &c_matrix, VectorXd &c_vector, size_t V, size_t P){
    bool Q = 0;
    for (int i{0}; i < P; i++){
        for (int k{0}; k < V; k++) {
            Q += pow((c_vector[i] - c_matrix(i,k)) , 2);

            if (Q > Q_critical) {
                return false;
            }
        }
    }
    return true;
}