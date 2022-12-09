//
// Created by dominic on 09/12/22.
//

#include "test_utils.h"


void print_test_results(string test_name, vector<TestCount> test_counts) {
    TestCount total_test_count;
    for (auto test_count : test_counts) {
        total_test_count.failed_tests += test_count.failed_tests;
        total_test_count.total_tests += test_count.total_tests;
    }
    if(total_test_count.failed_tests == 0) {
        cout << test_name << ": All tests passed." << endl;
    } else {
        cout << test_name << ": " << total_test_count.failed_tests << " out of " << total_test_count.total_tests << " tests failed."  << endl;
    }
  }
