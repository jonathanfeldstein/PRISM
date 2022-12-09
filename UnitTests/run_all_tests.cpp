#include "run_all_tests.h"

bool RunAllTests(string path_to_data) {
    TestCount test_count;
    TestCount test_hypergraph = TestHypergraph(path_to_data);
    test_count.total_tests += test_hypergraph.total_tests;
    test_count.failed_tests += test_hypergraph.failed_tests;
//    cout << "Hypergraph tests passed: " << pass_hypergraph.passed << "/" << pass_hypergraph.total << endl;
    TestCount test_undirected = TestUndirectedGraph(path_to_data);
    test_count.total_tests += test_undirected.total_tests;
    test_count.failed_tests += test_undirected.failed_tests;
    bool pass3 = TestHierarchicalClustering(path_to_data);
    TestStatistics();
    TestRandomWalks(path_to_data);
    bool pass4 = TestClustering();

    //        MatrixXd matrix_example(3,5);
//        matrix_example << 90, 60, 90, 30, 60, 90, 30, 60, 90, 90, 60, 60, 60, 30, 30;
//        MatrixXd result = PCA(matrix_example, 2);
//        cout << "Final Result" << endl;
//        cout << result << endl;

    return  pass3 && pass4;
}
