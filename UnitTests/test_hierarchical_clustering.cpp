#include "test_hierarchical_clustering.h"

vector<TestCount> TestHierarchicalClustering(const string& path_to_data) {
    vector<TestCount> test_results;
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

    cout << endl << "TESTING HIERARCHICAL CLUSTERING" << endl;
    test_results.push_back(test_clusters(hc_graph_clusters, config));
    test_results.push_back(test_no_nodes_lost(hc_graph_clusters, H));
    print_test_results("Hierarchical Clustering",test_results);
    return test_results;
}



TestCount test_clusters(const vector<UndirectedGraph>& graph_clusters, HierarchicalClustererConfig config) {
    TestCount test_cluster_results;
    test_cluster_results.test_name = "Testing Clusters after Hierarchical Clustering:";
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

TestCount test_no_nodes_lost(const vector<UndirectedGraph>& graph_clusters, HyperGraph H) {
    TestCount test_lost_nodes;
    test_lost_nodes.test_name = "Testing No Nodes Were Lost During Hierarchical Clustering:";
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

