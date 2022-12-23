#include "run_all_tests.h"

bool RunAllTests(const string& path_to_data) {
    pair<size_t, size_t> test_count_hypergraph = TestHypergraph(path_to_data);
    pair<size_t, size_t> test_count_undirected = TestUndirectedGraph(path_to_data);
    pair<size_t, size_t> test_count_hierarchcial_clustering = TestHierarchicalClustering(path_to_data);
    pair<size_t, size_t> test_count_statistics = TestStatistics();
    pair<size_t, size_t> test_count_random_walks = TestRandomWalks(path_to_data);
    pair<size_t, size_t> test_count_clustering = TestClustering();

    if (test_count_hypergraph.second == 0){
        cout << "All " << test_count_hypergraph.first << " hypergraph tests passed!" << endl;
    }else{
        cout << test_count_hypergraph.second << " out of " << test_count_hypergraph.first << " hypergraph tests failed!" << endl;
    }
    if (test_count_undirected.second == 0){
        cout << "All " << test_count_undirected.first << " undirected graph tests passed!" << endl;
    }else{
        cout << test_count_undirected.second << " out of " << test_count_undirected.first << " undirected graph tests failed!" << endl;
    }
    if (test_count_hierarchcial_clustering.second == 0){
        cout << "All " << test_count_hierarchcial_clustering.first << " hierarchical clustering tests passed!" << endl;
    }else{
        cout << test_count_hierarchcial_clustering.second << " out of " << test_count_hierarchcial_clustering.first << " hierarchical clustering tests failed!" << endl;
    }
    if (test_count_statistics.second == 0){
        cout << "All " << test_count_statistics.first << " statistics tests passed!" << endl;
    }else{
        cout << test_count_statistics.second << " out of " << test_count_statistics.first << " statistics tests failed!" << endl;
    }
    if (test_count_random_walks.second == 0){
        cout << "All " << test_count_random_walks.first << " random walk tests passed!" << endl;
    }else{
        cout << test_count_random_walks.second << " out of " << test_count_random_walks.first << " random walk tests failed!" << endl;
    }
    if (test_count_clustering.second == 0){
        cout << "All " << test_count_clustering.first << " random walk tests passed!" << endl;
    }else{
        cout << test_count_clustering.second << " out of " << test_count_clustering.first << " random walk tests failed!" << endl;
    }
    return  true;
}
