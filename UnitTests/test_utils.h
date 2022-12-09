#ifndef PRISM_TEST_UTILS_H
#define PRISM_TEST_UTILS_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct TestCount {
    string test_name;
    int total_tests = 0;
    int failed_tests = 0;
    vector<string> error_messages;
};

void print_test_results(const string& test_group_name, const vector<TestCount>& test_counts);



#endif //PRISM_TEST_UTILS_H
