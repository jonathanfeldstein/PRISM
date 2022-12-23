#include "test_utils.h"


void print_test_results(const string& test_group_name, const vector<TestReport>& test_counts) {
    TestReport total_test_count;
    for (const auto& test_count : test_counts) {
        total_test_count.failed_tests += test_count.failed_tests;
        total_test_count.total_tests += test_count.total_tests;
    }
    cout << test_group_name << ":" << endl;
    if(total_test_count.failed_tests == 0) {
        cout << "All " << total_test_count.total_tests << " tests passed." << endl;
    } else {
        cout << total_test_count.failed_tests << " out of "
             << total_test_count.total_tests << " tests failed."  << endl;
        cout << "Details:" << endl;
        for(const auto& test:test_counts){
            for(const auto& message:test.error_messages)
                cout << message << endl;
        }
    }
    cout << endl;
  }

vector<vector<size_t>> group_indices_by_value(const vector<size_t>& values) {
    vector<vector<size_t>> result;
    // Create a map that maps each value to a vector of indices where that value appears
    vector<size_t> label_order;
    map<size_t, vector<size_t>> index_groups;

    // Iterate over the input values and add their indices to the corresponding index group
    for (int i = 0; i < values.size(); ++i) {
        index_groups[values[i]].push_back(i);
        if(find(label_order.begin(),label_order.end(),values[i]) == label_order.end()){
            label_order.push_back(values[i]);
        }
    }

    // Extract the nested vectors from the map and return them as the result
    for (const auto& label : label_order) {
        result.emplace_back(index_groups[label]);
    }
    return result;
}

bool areNestedVectorsEqual(const vector<vector<size_t>>& vec1, const vector<vector<size_t>>& vec2)
{
    // Check if the sizes of the two vectors are different
    if (vec1.size() != vec2.size()) {
        return false;
    }

    // Compare the values in the two vectors
    for (int i = 0; i < vec1.size(); i++) {
        // Check if the sizes of the inner vectors are different
        if (vec1[i].size() != vec2[i].size()) {
            return false;
        }

        // Compare the values in the inner vectors
        for (int j = 0; j < vec1[i].size(); j++) {
            if (vec1[i][j] != vec2[i][j]) {
                return false;
            }
        }
    }

    // If all checks pass, the vectors are equal
    return true;
}

bool check_if_clustering_is_as_expected(const vector<size_t>& observed_clustering,
                                        const vector<size_t>& expected_clustering) {
    vector<vector<size_t>> grouping1 = group_indices_by_value(observed_clustering);
    vector<vector<size_t>> grouping2 = group_indices_by_value(expected_clustering);
    return areNestedVectorsEqual(grouping1,
                                 grouping2);
}


vector<size_t> get_clustering_labels_from_cluster_NP(const NodePartition& clusters, size_t number_of_nodes) {
    vector<size_t> clustering_label;
    for(size_t i = 0; i<number_of_nodes; i++){
        clustering_label.push_back(0);
    }
    size_t cluster_id = 0;

    // single nodes
    for (auto node_id: clusters.single_nodes) {
        clustering_label[node_id] = cluster_id;
        cluster_id++;
    }

    // clusters
    for (const auto& cluster: clusters.clusters) {
        for (const auto& node_id: cluster) {
            clustering_label[node_id] = cluster_id;
        }
        cluster_id++;
    }

    return clustering_label;
}

vector<size_t> get_clustering_labels_from_cluster_RW(const RandomWalkNodePartition& clusters, size_t number_of_nodes) {
    vector<size_t> clustering_label;
    for(size_t i = 0; i<number_of_nodes; i++){
        clustering_label.push_back(0);
    }
    size_t cluster_id = 0;

    // single nodes
    for (auto node_id: clusters.single_nodes) {
        clustering_label[node_id] = cluster_id;
        cluster_id++;
    }

    // clusters
    for (const auto& cluster: clusters.clusters) {
        for (const auto& node: cluster) {
            NodeId node_id = node.get_node_id();
            clustering_label[node_id] = cluster_id;
        }
        cluster_id++;
    }

    return clustering_label;
}

