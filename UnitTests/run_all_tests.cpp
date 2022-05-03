//
// Created by dominic on 01/05/22.
//

#include "run_all_tests.h"

bool RunAllTests(string path_to_data) {
    bool pass1 = TestHypergraph(path_to_data);
    bool pass2 = TestUndirectedGraph(path_to_data);
    bool pass3 = TestHierarchicalClustering(path_to_data);
    TestStatistics();

    return  pass1 && pass2 && pass3;
}
