#include "run_all_tests.h"

bool RunAllTests(const string& path_to_data) {
    TestCount test_count;
    TestCount test_hypergraph = TestHypergraph(path_to_data);
    test_count.total_tests += test_hypergraph.total_tests;
    test_count.failed_tests += test_hypergraph.failed_tests;
    TestCount test_undirected = TestUndirectedGraph(path_to_data);
    test_count.total_tests += test_undirected.total_tests;
    test_count.failed_tests += test_undirected.failed_tests;
    vector<TestCount> hierarchcial_clustering_test_results = TestHierarchicalClustering(path_to_data);
    TestStatistics();
    TestRandomWalks(path_to_data);
    bool pass4 = TestClustering();

    //        MatrixXd matrix_example(3,5);
//        matrix_example << 90, 60, 90, 30, 60, 90, 30, 60, 90, 90, 60, 60, 60, 30, 30;
//        MatrixXd result = PCA(matrix_example, 2);
//        cout << "Final Result" << endl;
//        cout << result << endl;

    return  pass4;
}
