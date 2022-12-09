#include "test_utils.h"


void print_test_results(const string& test_group_name, const vector<TestCount>& test_counts) {
    TestCount total_test_count;
    for (const auto& test_count : test_counts) {
        total_test_count.failed_tests += test_count.failed_tests;
        total_test_count.total_tests += test_count.total_tests;
    }
    cout << test_group_name << ":" << endl;
    if(total_test_count.failed_tests == 0) {
        cout << "All tests passed." << endl;
    } else {
        cout << total_test_count.failed_tests << " out of "
             << total_test_count.total_tests << " tests failed."  << endl;
        cout << "Details:" << endl;
        for(const auto& test:test_counts){
            for(const auto& message:test.error_messages)
                cout << message << endl;
        }
    }
  }
