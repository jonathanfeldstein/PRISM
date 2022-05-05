//
// Created by jonathan on 04/05/2022.
//

#include "test_clustering.h"

bool TestClustering() {
    bool state = true;
    if(!test_theta_sym()){
        state=false;
    }

    MatrixXd matrix_example(3,5);
    matrix_example << 90, 60, 90, 30, 60, 90, 30, 60, 90, 90, 60, 60, 60, 30, 30;
    MatrixXd solution(3, 2);
    solution << 0.5483, -1.4257, -2.0463, 0.3820, 1.4980, 1.0437;
//    MatrixXd standardized_example;
//    VectorXd mean_vector;
//    mean_vector = matrix_example.rowwise().mean();
//    cout << "Mean vector" ;
//    cout << mean_vector << endl;
//    cout << "Subtracted mean" << endl;
//    cout << matrix_example.colwise() - mean_vector;
//    cout << "Squared subtracted mean" << endl;
//    cout << (matrix_example.colwise() - mean_vector).array().pow(2);
//    cout << "Sum of rows" << endl;
//    cout << (matrix_example.colwise() - mean_vector).array().pow(2).rowwise().sum();
//    cout << "Sqrt" << endl;
//    cout << (matrix_example.colwise() - mean_vector).array().pow(2).rowwise().sum().cwiseSqrt();
//    cout << "Normalised" << endl;
//    cout << (matrix_example.colwise() - mean_vector).array().pow(2).rowwise().sum().cwiseSqrt() / pow((matrix_example.cols()-1), 0.5);
//    VectorXd std_dev = (matrix_example.colwise() - mean_vector).array().pow(2).rowwise().sum().cwiseSqrt() / pow((matrix_example.cols()-1), 0.5);
//    VectorXd inverse_std_vector = std_dev.cwiseInverse();
//    cout << "STD Vector" << endl;
//    cout << std_dev;
//    cout << "Inverse STD vector" ;
//    cout << inverse_std_vector << endl;
//    MatrixXd diagonal = inverse_std_vector.asDiagonal();
//    cout << "Diagonal inverse STD vector" ;
//    cout << diagonal << endl;
//    standardized_example =  diagonal * (matrix_example.colwise() - mean_vector);
//    cout << "Standardized example" ;
//    cout << standardized_example << endl;
    if(!test_pca(matrix_example, solution)){
        state = false;
    }


    vector<NodeRandomWalkData> sample_path_data;
    NodeRandomWalkData node_1_data;
    node_1_data.add_path("1, 1");
    node_1_data.add_path("1, 1");
    node_1_data.add_path("1, 1");
    node_1_data.add_path("2, 2, 2");
    node_1_data.add_path("2, 2, 2");
    node_1_data.add_path("3, 3, 3");
    node_1_data.add_path("4, 4, 4, 4");
    NodeRandomWalkData node_2_data;
    node_2_data.add_path("3");
    node_2_data.add_path("3");
    node_2_data.add_path("3");
    node_2_data.add_path("2, 2");
    node_2_data.add_path("2, 2");
    node_2_data.add_path("1, 1, 1");
    node_2_data.add_path("4, 4, 4, 4");
    sample_path_data.emplace_back(node_1_data);
    sample_path_data.emplace_back(node_2_data);

    MatrixXd expected_top_paths1{2,3};
    expected_top_paths1 << 2, 1, 0, 0, 0, 1;
    test_compute_top_paths(sample_path_data, 3, 3, expected_top_paths1);

    MatrixXd expected_top_paths2{2,2};
    expected_top_paths2 << 2, 0, 0, 1;
    test_compute_top_paths(sample_path_data, 1, 3, expected_top_paths2);

    MatrixXd expected_top_paths3{2,2};
    expected_top_paths3 << 3, 0, 0, 2;
    test_compute_top_paths(sample_path_data, 3, 2, expected_top_paths3);

    MatrixXd expected_top_paths4{2,1};
    expected_top_paths4 << 0, 3;
    test_compute_top_paths(sample_path_data, 3, 1, expected_top_paths4);

    MatrixXd expected_top_paths5{2,4};
    expected_top_paths5 << 3, 2, 0, 0, 0, 0, 3, 2;
    test_compute_top_paths(sample_path_data, 2, 0, expected_top_paths5);

    MatrixXd expected_top_paths6{2,7};
    expected_top_paths6 << 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 1, 3, 2, 1;
    test_compute_top_paths(sample_path_data, 10, 0, expected_top_paths6);

    vector<size_t> expected_cluster_labels{2, 2, 1, 1, 1};
    MatrixXd all_points(5,2);
    all_points << -1, -1.5, -1.5, -1, 1, 1.25, 1.5, 1, 1.75, 1.5;
    vector<size_t> expected_cluster_labels2{3, 4, 1, 1, 1};

    test_two_means(all_points, expected_cluster_labels, expected_cluster_labels2);

    MatrixXd h2m_npc_test(5,3);
    h2m_npc_test << 100, 75, 50, 103, 72, 51, 101, 74, 49, 35, 105, 10, 12, 17, 90;

    MatrixXd h2m_fv_test = compute_principal_components(h2m_npc_test,2);
    vector<size_t> expected_hierarchical_cluster_labels{1, 1, 1, 2, 3};
    test_hierarchical_two_means(h2m_npc_test, h2m_fv_test, expected_hierarchical_cluster_labels);



    return state;
}


bool test_theta_sym(){
    double theta1 = compute_theta_sym(0.01, 5, 5);
    if(abs(theta1 -5.82377)>0.001){
        std::cout<<"Theta Sym computed incorrectly." <<endl;
        std::cout<<"Expected: 5.82377" << "Actual: " << theta1;
        return false;
    }
    cout << ".";
    double theta2 = compute_theta_sym(0.01, 1000, 5);
    if(abs(theta2 -0.23097)>0.001){
        std::cout<<"Theta Sym computed incorrectly." <<endl;
        std::cout<<"Expected: 0.23097" << "Actual: " << theta2;
        return false;
    }
    cout << ".";
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
    cout << ".";
    return true;
}

bool test_compute_top_paths(vector<NodeRandomWalkData> sample_path_data, size_t max_num_paths, size_t path_length, MatrixXd expected_top_paths) {

    MatrixXd computed_top_paths = compute_top_paths(sample_path_data, max_num_paths, path_length);
    if (!computed_top_paths.isApprox(expected_top_paths)) {
        cout << "Top paths was computed incorrectly" << endl;
        cout << "Obtained paths: " << computed_top_paths << endl;
        cout << "Expected paths: " << expected_top_paths << endl;
    }
    cout << ".";

}

bool test_two_means(MatrixXd all_points, vector<size_t> expected_cluster_labels_after_one_iteration, vector<size_t> expected_after_two_iterations) {

    vector<size_t> cluster_labels(5,0);
    two_means(cluster_labels, all_points, 10, 0.01, 0);

    if (cluster_labels != expected_cluster_labels_after_one_iteration) {
        cout << "Expected cluster labels:" << endl;
        for (auto label: expected_cluster_labels_after_one_iteration) {
            cout << label;
        }
        cout << endl;

        cout << "Actual cluster labels: " << endl;
        for (auto label: cluster_labels) {
            cout << label;
        }
        cout << endl;
        return false;
    }
    cout << ".";



    two_means(cluster_labels, all_points, 10, 0.01, 2);

    if (cluster_labels != expected_after_two_iterations) {
        cout << "Expected cluster labels:" << endl;
        for (auto label: expected_after_two_iterations) {
            cout << label;
        }
        cout << endl;

        cout << "Actual cluster labels: " << endl;
        for (auto label: cluster_labels) {
            cout << label;
        }
        cout << endl;
        return false;
    }
    cout << ".";

    return true;

}


bool test_hierarchical_two_means(MatrixXd npc, MatrixXd nfv, vector<size_t> expected_cluster_labels) {
    int number_of_walks = 300;
    double significance_level = 0.01;

    vector<size_t> calculated_cluster_labels = hierarchical_two_means(npc,
                           nfv,
                           10, //TODO find a theoretical reason for 10 and 0.01, DOM!!!
                           0.01,
                           number_of_walks,
                           significance_level);

    if (calculated_cluster_labels != expected_cluster_labels) {
        cout << "Hierarchical two means clustering labels do not match expected values" << endl;

        cout << "Expected cluster labels:" << endl;
        for (auto label: expected_cluster_labels) {
            cout << label;
        }
        cout << endl;

        cout << "Actual cluster labels: " << endl;
        for (auto label: calculated_cluster_labels) {
            cout << label;
        }
        cout << endl;
        return false;

    }

    cout << ".";


}
