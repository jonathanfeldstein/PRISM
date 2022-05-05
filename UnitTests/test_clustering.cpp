//
// Created by jonathan on 04/05/2022.
//

#include "test_clustering.h"

bool TestClustering() {
    MatrixXd matrix_example(5,3);
    matrix_example << 90, 90, 60, 60, 30, 60, 90, 60, 60, 30, 90, 30, 60, 90, 30;
    MatrixXd solution(2, 3);
    solution << 30.37, 13.67, 9.98, -47.7, -3.93, 2.31;
    MatrixXd standardized_example;
    VectorXd mean_vector;
    mean_vector = matrix_example.rowwise().mean();
    standardized_example =  mean_vector.inverse().asDiagonal() * (matrix_example - mean_vector);
    test_pca(matrix_example, solution);
}


bool test_theta_sym(){
    double theta1 = compute_theta_sym(0.01, 5, 5);
    if(abs(theta1 -5.82377)>0.001){
        std::cout<<"Theta Sym computed incorrectly." <<endl;
        std::cout<<"Expected: 5.82377" << "Actual: " << theta1;
        return false;
    }
    double theta2 = compute_theta_sym(0.01, 1000, 5);
    if(abs(theta2 -0.23097)>0.001){
        std::cout<<"Theta Sym computed incorrectly." <<endl;
        std::cout<<"Expected: 0.23097" << "Actual: " << theta2;
        return false;
    }
    return true;
}

bool test_pca(MatrixXd matrix_example, MatrixXd solution){
    MatrixXd computed_pca = compute_principal_components(matrix_example, 2);
    if(!computed_pca.isApprox(solution)){
        std::cout<< "PCA failed." << endl;
        std::cout<< "Expected: "<< endl;
        std::cout<< solution << endl;
        std::cout<< "Actual: " << endl;
        std::cout<< computed_pca;
        return false;
    }
}