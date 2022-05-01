//
// Created by dominic on 01/05/22.
//

#include "run_all_tests.h"

bool RunAllTests() {
    bool pass1 = TestHypergraph();
    bool pass2 = TestUndirectedGraph();
    bool pass3 = TestHierarchicalClustering();

    return pass1 && pass2 && pass3;
}
