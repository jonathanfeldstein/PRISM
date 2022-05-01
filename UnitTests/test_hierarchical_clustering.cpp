//
// Created by dominic on 01/05/22.
//

#include "test_hierarchical_clustering.h"

bool TestHierarchicalClustering(string path_to_data) {
    bool state = true;
    string imdb_db = path_to_data+"/imdb1.db";
    string imdb_info = path_to_data+"/imdb.info";

    HyperGraph H = HyperGraph(imdb_db, imdb_info);
    HierarchicalClustererConfig config;
    config.min_cluster_size = 3;
    config.max_lambda2 = 0.7;
    HierarchicalClusterer hc = HierarchicalClusterer(H, config);

    vector<HyperGraph> hc_hypergraph_clusters = hc.run_hierarchical_clustering();
    vector<UndirectedGraph> hc_graph_clusters = hc.get_graph_clusters();

    if(!test_min_cluster_size(hc_graph_clusters, config)){
        state = false;
        cout << "FAILED in test_min_cluster_size" << endl;
    }
    if(!test_max_lambda2(hc_graph_clusters, config)){
        state = false;
        cout << "FAILED in test_max_lambda2" << endl;
    }
    if(!test_no_nodes_lost(hc_graph_clusters, H)){
        state = false;
        cout << "FAILED in test_no_nodes_lost" <<endl;
    }
    return state;
}

bool test_min_cluster_size(vector<UndirectedGraph> graph_clusters, HierarchicalClustererConfig config) {
    for (UndirectedGraph graph: graph_clusters) {
        if (graph.number_of_nodes() <= config.min_cluster_size) {
            cout << "Graph created with number of node (" << graph.number_of_nodes()
            << ") being not larger than the minimum cluster size (" << config.min_cluster_size << ")" << endl;
            return false;
        }
        cout << ".";
    }
    return true;
}


bool test_max_lambda2(vector<UndirectedGraph> graph_clusters, HierarchicalClustererConfig config) {
    for(auto graph: graph_clusters){
        if (graph.get_second_eigenpair().second > config.max_lambda2) {
            cout << "Graph created has a lambda 2 ("
            << graph.get_second_eigenpair().second
            << "), which is larger than the max lambda 2 specified by the config file ("
            << config.max_lambda2 << endl;
            return false;
        }
        cout << ".";
    }
    return true;
}

bool test_no_nodes_lost(vector<UndirectedGraph> graph_clusters, HyperGraph H) {
    size_t num_nodes;
    for (UndirectedGraph graph: graph_clusters) {
        num_nodes += graph.number_of_nodes();
    }
    if(num_nodes != H.number_of_nodes()){
        cout << "Expected #nodes: "<< H.number_of_nodes() << ", Actual "<< num_nodes << endl;
        return false;
    }
    cout << ".";
    return true;
}

