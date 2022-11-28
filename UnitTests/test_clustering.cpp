#include "test_clustering.h"

bool TestClustering() {
    bool state = true;
    if(!test_theta_sym()){
        state=false;
    }

    MatrixXd matrix_example(3,5);
    matrix_example << 90, 60, 90, 30, 60, 90, 30, 60, 90, 90, 60, 60, 60, 30, 30;
    MatrixXd solution(3, 2);
    solution << 0.671537, -1.74615, -2.50621, 0.467879, 1.83467, 1.27827;
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


    vector<NodeRandomWalkData> tht_path_data;
    NodeRandomWalkData node_1_tht(1, "node", 3);
    NodeRandomWalkData node_2_tht(2, "node", 3.2);
    NodeRandomWalkData node_3_tht(3, "node", 3.7);
    NodeRandomWalkData node_4_tht(4, "node", 3.9);
    NodeRandomWalkData node_5_tht(5, "node", 5);
    tht_path_data.emplace_back(node_3_tht);
    tht_path_data.emplace_back(node_1_tht);
    tht_path_data.emplace_back(node_2_tht);
    tht_path_data.emplace_back(node_4_tht);
    tht_path_data.emplace_back(node_5_tht);

    vector<size_t> expected_clustering{0,1,1,0,2};

    test_cluster_nodes_by_truncated_hitting_times(tht_path_data, 0.25, expected_clustering);

    vector<NodeRandomWalkData> skd_path_data;
    NodeRandomWalkData node_1_skd(1, "node", 3);
    NodeRandomWalkData node_2_skd(2, "node", 3.2);
    NodeRandomWalkData node_3_skd(3, "node", 3.7);
    NodeRandomWalkData node_4_skd(4, "node", 3.9);
    NodeRandomWalkData node_5_skd(5, "node", 5);
    node_1_skd.add_path("1,1,1");node_1_skd.add_path("1,1,1");node_1_skd.add_path("1,1,1");node_1_skd.add_path("1,1,1");
    node_1_skd.add_path("2,2,2");node_1_skd.add_path("2,2,2");node_1_skd.add_path("2,2,2");node_1_skd.add_path("2,2,2");
    node_1_skd.add_path("3,3,3");node_1_skd.add_path("3,3,3");node_1_skd.add_path("3,3,3");node_1_skd.add_path("3,3,3");

    node_2_skd.add_path("1,1,1");node_2_skd.add_path("1,1,1");node_2_skd.add_path("1,1,1");node_2_skd.add_path("1,1,1");
    node_2_skd.add_path("2,2,2");node_2_skd.add_path("2,2,2");node_2_skd.add_path("2,2,2");
    node_2_skd.add_path("3,3,3");node_2_skd.add_path("3,3,3");node_2_skd.add_path("3,3,3");node_2_skd.add_path("3,3,3");

    node_3_skd.add_path("1,1,1");node_3_skd.add_path("1,1,1");node_3_skd.add_path("1,1,1");node_3_skd.add_path("1,1,1");
    node_3_skd.add_path("2,2,2");node_3_skd.add_path("3,3,3");node_3_skd.add_path("3,3,3");node_3_skd.add_path("3,3,3");
    node_3_skd.add_path("3,3,3");node_3_skd.add_path("3,3,3");node_3_skd.add_path("3,3,3");node_3_skd.add_path("3,3,3");

    node_4_skd.add_path("1,1,1");node_4_skd.add_path("1,1,1");node_4_skd.add_path("1,1,1");node_4_skd.add_path("1,1,1");
    node_4_skd.add_path("2,2,2");node_4_skd.add_path("3,3,3");node_4_skd.add_path("3,3,3");node_4_skd.add_path("3,3,3");
    node_4_skd.add_path("3,3,3");node_4_skd.add_path("3,3,3");node_4_skd.add_path("3,3,3");

    node_5_skd.add_path("1,1");node_5_skd.add_path("1,1");node_5_skd.add_path("1,1");node_5_skd.add_path("1,1");
    node_5_skd.add_path("2,2");node_5_skd.add_path("2,2");node_5_skd.add_path("2,2");node_5_skd.add_path("2,2");
    node_5_skd.add_path("3,3");node_5_skd.add_path("3,3");node_5_skd.add_path("3,3");node_5_skd.add_path("3,3");

    skd_path_data.emplace_back(node_1_skd);
    skd_path_data.emplace_back(node_2_skd);
    skd_path_data.emplace_back(node_3_skd);
    skd_path_data.emplace_back(node_4_skd);
    skd_path_data.emplace_back(node_5_skd);

    vector<size_t> sk_expected_clustering1{1,1,0,0,2};
//    test_cluster_nodes_by_SK_divergence(skd_path_data, 0.01, 15, 10, sk_expected_clustering1);
//
//    vector<size_t> sk_expected_clustering2{0,0,0,0,1};
//    test_cluster_nodes_by_SK_divergence(skd_path_data, 0.0000001, 15, 10, sk_expected_clustering2);
//
//    vector<size_t> sk_expected_clustering3{1,2,3,4,5};
//    test_cluster_nodes_by_SK_divergence(skd_path_data, 0.99, 15, 10, sk_expected_clustering3);
//
//    vector<size_t> birch_expected_clustering1{1,1,0,0,2};
//    test_cluster_nodes_by_birch(skd_path_data, 2, 10, 15, 0.05, birch_expected_clustering1);
//
//    vector<size_t> birch_expected_clustering2{0,0,0,0,1};
//    test_cluster_nodes_by_birch(skd_path_data, 2, 10, 15, 0.011, birch_expected_clustering2);
//
//    vector<size_t> birch_expected_clustering3{1,2,3,4,5};
//    test_cluster_nodes_by_birch(skd_path_data, 2, 10, 15, 0.99, birch_expected_clustering3);


    RandomWalkerConfig config;
    config.max_random_walk_length=5;
    config.num_top_paths_for_clustering=3;
    config.epsilon=0.05;
    config.alpha=0.05;
    config.pca_dim=2;
    //config.clustering_method_threshold=50;
    test_cluster_nodes_by_path_distribution(skd_path_data,15,3,config,sk_expected_clustering1);

    //config.clustering_method_threshold=0;
    test_cluster_nodes_by_path_distribution(skd_path_data,15,3,config,sk_expected_clustering1);

    //config.clustering_method_threshold=50;
    test_cluster_nodes_by_path_similarity(skd_path_data, 15, 3,0.2, config, sk_expected_clustering1);

    //config.clustering_method_threshold=0;
    test_cluster_nodes_by_path_similarity(skd_path_data, 15, 3, 0.2,config, sk_expected_clustering1);

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
    if(!computed_pca.isApprox(solution,1e-3)){
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

    return true;
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
                           10,
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

    return true;
}


bool test_cluster_nodes_by_truncated_hitting_times(vector<NodeRandomWalkData> nodes_of_type,
                                                   double threshold_hitting_time_difference,
                                                   vector<size_t> expected_clustering) {

    pair<set<NodeId>, vector<vector<NodeRandomWalkData>>> clusters = cluster_nodes_by_truncated_hitting_times(nodes_of_type, threshold_hitting_time_difference);

    cout << "Expected clustering" << endl;
    int i = 0;
    for (auto node: nodes_of_type) {
        cout << "Node ID: " << node.get_node_id() << " Expected cluster label: " << expected_clustering[i] << endl;
        i ++;
    }

    // single nodes
    set<NodeId> single_nodes = clusters.first;
    cout << "Single Nodes" << endl;
    for (auto node: single_nodes) {
        cout << node << endl;
    }
    // clusters
    vector<vector<NodeRandomWalkData>> clusts = clusters.second;
    cout << "Clusters" << endl;
    int j = 0;
    for (auto cluster: clusts) {
        cout << "Cluster " << j << endl;
        for (auto node: cluster) {
            cout << node.get_node_id() << endl;
        }
        j++;
    }

    return true;


}


bool test_cluster_nodes_by_SK_divergence(const vector<NodeRandomWalkData> &nodes_of_type, double significance_level, size_t number_of_walks, size_t max_number_of_paths, vector<size_t> expected_clustering) {

    NodePartition calculated_sk_clusters = cluster_nodes_by_sk_divergence(nodes_of_type,significance_level,number_of_walks,max_number_of_paths);

    cout << "Expected clustering" << endl;
    int i{0};
    for (auto node: nodes_of_type) {
        cout << "Node ID: " << node.get_node_id() << " Expected cluster label: " << expected_clustering[i] << endl;
        i ++;
    }

    // single nodes
    set<NodeId> single_nodes = calculated_sk_clusters.single_nodes;
    cout << "Single Nodes" << endl;
    for (auto node: single_nodes) {
        cout << node << endl;
    }
    // clusters
    vector<Cluster> clusts = calculated_sk_clusters.clusters;
    cout << "Clusters" << endl;
    for (const Cluster& cluster: clusts) {
        cout << "Cluster " << endl;
        for (auto node: cluster) {
            cout << node << endl;
        }
    }
    return true;
}


bool test_cluster_nodes_by_birch(const vector<NodeRandomWalkData> &nodes,
                                 int pca_target_dimension,
                                 int max_number_of_paths,
                                 int number_of_walks,
                                 double significance_level,
                                 vector<size_t> expected_clustering) {

    NodePartition calculated_clustering = cluster_nodes_by_birch(nodes, pca_target_dimension, max_number_of_paths,number_of_walks,significance_level);

    cout << "Expected clustering" << endl;
    int i{0};
    for (auto node: nodes) {
        cout << "Node ID: " << node.get_node_id() << " Expected cluster label: " << expected_clustering[i] << endl;
        i ++;
    }

    // single nodes
    set<NodeId> single_nodes = calculated_clustering.single_nodes;
    cout << "Single Nodes" << endl;
    for (auto node: single_nodes) {
        cout << node << endl;
    }
    // clusters
    vector<Cluster> clusts = calculated_clustering.clusters;
    cout << "Clusters" << endl;
    for (const Cluster& cluster: clusts) {
        cout << "Cluster " << endl;
        for (auto node: cluster) {
            cout << node << endl;
        }
    }
    return true;
}

bool test_cluster_nodes_by_path_distribution(const vector<NodeRandomWalkData> &nodes_of_type,
                                   size_t number_of_walks,
                                   size_t length_of_walks,
                                   RandomWalkerConfig &config,
                                   vector<size_t> expected_clustering) {


    NodePartition path_distribution_clusters = cluster_nodes_by_path_distribution(nodes_of_type,number_of_walks,length_of_walks,config);

    cout << "Expected clustering" << endl;
    int i{0};
    for (auto node: nodes_of_type) {
        cout << "Node ID: " << node.get_node_id() << " Expected cluster label: " << expected_clustering[i] << endl;
        i ++;
    }

    // single nodes
    set<NodeId> single_nodes = path_distribution_clusters.single_nodes;
    cout << "Single Nodes" << endl;
    for (auto node: single_nodes) {
        cout << node << endl;
    }
    // clusters
    vector<Cluster> clusts = path_distribution_clusters.clusters;
    cout << "Clusters" << endl;
    for (const Cluster& cluster: clusts) {
        cout << "Cluster " << endl;
        for (auto node: cluster) {
            cout << node << endl;
        }
    }
    return true;

}





bool test_cluster_nodes_by_path_similarity(const vector<NodeRandomWalkData> &nodes_of_type,
                                             size_t number_of_walks,
                                             size_t length_of_walks,
                                             double theta_sym,
                                             RandomWalkerConfig &config,
                                             vector<size_t> expected_clustering) {


    NodePartition path_similarity_clusters = cluster_nodes_by_path_similarity(nodes_of_type,number_of_walks,length_of_walks,theta_sym,config);

    cout << "Expected clustering" << endl;
    int i{0};
    for (auto node: nodes_of_type) {
        cout << "Node ID: " << node.get_node_id() << " Expected cluster label: " << expected_clustering[i] << endl;
        i ++;
    }

    // single nodes
    set<NodeId> single_nodes = path_similarity_clusters.single_nodes;
    cout << "Single Nodes" << endl;
    for (auto node: single_nodes) {
        cout << node << endl;
    }
    // clusters
    vector<Cluster> clusts = path_similarity_clusters.clusters;
    cout << "Clusters" << endl;
    for (const Cluster& cluster: clusts) {
        cout << "Cluster " << endl;
        for (auto node: cluster) {
            cout << node << endl;
        }
    }
    return true;

}