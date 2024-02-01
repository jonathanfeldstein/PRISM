#ifndef PRISM_TEST_UTILS_H
#define PRISM_TEST_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include "../Graphs/Relation.h"
#include "../Concepts/Partition.h"

using namespace std;

struct TestReport {
    int total_tests = 0;
    int failed_tests = 0;
    vector<string> error_messages{};
    TestReport() = default;
    ~TestReport() = default;
};

void print_test_results(const string& test_group_name, const vector<TestReport>& test_counts);

vector<vector<size_t>> group_indices_by_value(const vector<size_t>& values);

bool areNestedVectorsEqual(const vector<vector<size_t>>& vec1, const vector<vector<size_t>>& vec2);

bool check_if_clustering_is_as_expected(const vector<size_t>& observed_clustering,
                                        const vector<size_t>& expected_clustering);

vector<size_t> get_clustering_labels_from_cluster_NP(const NodePartition& clusters, size_t number_of_nodes);
vector<size_t> get_clustering_labels_from_cluster_RW(const RandomWalkNodePartition& clusters, size_t number_of_nodes);

#endif //PRISM_TEST_UTILS_H
