#include "test_hierarchical_clustering.h"

pair<size_t, size_t> TestHierarchicalClustering(const string& path_to_data) {
    pair<size_t, size_t> test_count{}; // Count of {total tests, failed tests}.
    bool state = true;
    string imdb_db = path_to_data+"/imdb1.db";
    string imdb_info = path_to_data+"/imdb1.info";

    HyperGraph H = HyperGraph(imdb_db, imdb_info, true);
    HierarchicalClustererConfig config;
    config.min_cluster_size = 3;
    config.max_lambda2 = 0.7;
    HierarchicalClusterer hc = HierarchicalClusterer(H, config);

    vector<HyperGraph> hc_hypergraph_clusters = hc.run_hierarchical_clustering();
    vector<UndirectedGraph> hc_graph_clusters = hc.get_graph_clusters();
    cout << "------------------------------------------"<<endl;
    cout << endl << "TESTING HIERARCHICAL CLUSTERING" << endl;
    TestReport clusters_test = test_clusters(hc_graph_clusters, config);
    TestReport no_nodes_lost_test = test_no_nodes_lost(hc_graph_clusters, H);
    print_test_results("Hierarchical Clustering", {clusters_test, no_nodes_lost_test});
    test_count.first += clusters_test.total_tests + no_nodes_lost_test.total_tests;
    test_count.second += clusters_test.failed_tests + no_nodes_lost_test.failed_tests;
    return test_count;
}



TestReport test_clusters(const vector<UndirectedGraph>& graph_clusters, HierarchicalClustererConfig config) {
    TestReport test_cluster_results;
    for(auto graph: graph_clusters){
        if (graph.get_second_eigenpair().second < config.max_lambda2 && graph.number_of_nodes() < config.min_cluster_size) {
            string message = "Graph created lambda 2 ";
            message += to_string(graph.get_second_eigenpair().second) +
                    " (config max lambda2 is " + to_string(config.max_lambda2) +
                    ") and has size " + to_string(graph.number_of_nodes()) +
                    " (config min cluster size " + to_string(config.min_cluster_size) +
                    ")" + "\n";
            test_cluster_results.error_messages.push_back(message);
            test_cluster_results.failed_tests++;
        }
    }
    test_cluster_results.total_tests++;
    return test_cluster_results;
}

TestReport test_no_nodes_lost(const vector<UndirectedGraph>& graph_clusters, HyperGraph H) {
    TestReport test_lost_nodes;
    size_t num_nodes{0};
    for (UndirectedGraph graph: graph_clusters) {
        num_nodes += graph.number_of_nodes();
    }
    if(num_nodes != H.number_of_nodes()){
        string message = "Expected #nodes: ";
        message += to_string(H.number_of_nodes()) + ", Actual "+ to_string(num_nodes) + "\n";
        test_lost_nodes.error_messages.push_back(message);
        test_lost_nodes.failed_tests++;
    }
    test_lost_nodes.total_tests++;
    return test_lost_nodes;
}

