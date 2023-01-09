#include "run_all_tests.h"


bool RunAllTests(const string& path_to_data) {
    bool all_tests_passed = true;
    vector<pair<size_t,size_t>> tests;
    pair<size_t, size_t> test_count_hypergraph = TestHypergraph(path_to_data);
    tests.push_back(test_count_hypergraph);
    pair<size_t, size_t> test_count_undirected = TestUndirectedGraph(path_to_data);
    tests.push_back(test_count_undirected);
    pair<size_t, size_t> test_count_hierarchcial_clustering = TestHierarchicalClustering(path_to_data);
    tests.push_back(test_count_hierarchcial_clustering);
    pair<size_t, size_t> test_count_statistics = TestStatistics();
    tests.push_back(test_count_statistics);
    pair<size_t, size_t> test_count_random_walks = TestRandomWalks(path_to_data);
    tests.push_back(test_count_random_walks);
    pair<size_t, size_t> test_count_clustering = TestClustering();
    tests.push_back(test_count_clustering);

    print_full_test_report({"Hypergraph Tests",
                                      "Graph Tests",
                                      "Hierarchical Clustering Tests",
                                      "Statistics Tests",
                                      "Random Walk Tests",
                                      "Clustering Tests"},
                          tests);
    for(auto test:tests){
        if(test.second != 0){
            all_tests_passed = false;
            cout << "Look above for details on the tests!" << endl;
        }
    }
    return  all_tests_passed;
}
