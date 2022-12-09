#include "run_all_tests.h"

bool RunAllTests(string path_to_data) {
    bool pass_hypergraph = TestHypergraph(path_to_data);
    if(pass_hypergraph){
        cout << "All Tests On Hypergraphs succeeded." << endl;
    }else{
        cout << "Some or all Tests On Hypergraphs succeeded." << endl;
    }
    bool pass2 = TestUndirectedGraph(path_to_data);
    bool pass3 = TestHierarchicalClustering(path_to_data);
    TestStatistics();
    TestRandomWalks(path_to_data);
    bool pass4 = TestClustering();

    //        MatrixXd matrix_example(3,5);
//        matrix_example << 90, 60, 90, 30, 60, 90, 30, 60, 90, 90, 60, 60, 60, 30, 30;
//        MatrixXd result = PCA(matrix_example, 2);
//        cout << "Final Result" << endl;
//        cout << result << endl;

    return  pass1 && pass2 && pass3 && pass4;
}
