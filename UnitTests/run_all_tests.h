#ifndef PRISM_RUN_ALL_TESTS_H
#define PRISM_RUN_ALL_TESTS_H

#include "test_graphs.h"
#include "test_hierarchical_clustering.h"
#include "test_statistics.h"
#include "test_random_walks.h"
#include "test_clustering.h"

bool RunAllTests(const string& path_to_data);

void print_full_test_report(vector<string> test_names, vector<pair<size_t,size_t>> test_results);

#endif //PRISM_RUN_ALL_TESTS_H
