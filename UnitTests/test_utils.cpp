#include "test_utils.h"


void print_test_results(const string& test_group_name, const vector<TestCount>& test_counts) {
    TestCount total_test_count;
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
    // Create a map that maps each value to a vector of indices where that value appears
    unordered_map<size_t, vector<size_t>> index_groups;

    // Iterate over the input values and add their indices to the corresponding index group
    for (int i = 0; i < values.size(); ++i) {
        index_groups[values[i]].push_back(i);
    }

    // Extract the nested vectors from the map and return them as the result
    vector<vector<size_t>> result;
    for (const auto& [value, indices] : index_groups) {
        result.push_back(indices);
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
    return areNestedVectorsEqual(group_indices_by_value(observed_clustering),
                                 group_indices_by_value(expected_clustering));
}


vector<size_t> get_clustering_labels_from_cluster(NodePartition clusters, size_t number_of_nodes) {
    vector<size_t> clustering_label(number_of_nodes,0);
    size_t cluster_id = 0;

    // single nodes
    set<NodeId> single_nodes = clusters.first;
    for (auto node_id: single_nodes) {
        clustering_label[node_id] = cluster_id;
        cluster_id++;
    }

    // clusters
    vector<vector<NodeRandomWalkData>> clusts = clusters.second;
    for (const auto& cluster: clusts) {
        for (const auto& node: cluster) {
            size_t node_id = node.get_node_id();
            clustering_label[node_id] = cluster_id;
        }
        cluster_id++;
    }

    return clustering_label;
}

vector<size_t> get_clustering_labels_from_cluster(NodePartition clusters, size_t number_of_nodes) {
    vector<size_t> clustering_label(number_of_nodes,0);
    size_t cluster_id = 0;

    // single nodes
    set<NodeId> single_nodes = clusters.first;
    for (auto node_id: single_nodes) {
        clustering_label[node_id] = cluster_id;
        cluster_id++;
    }

    // clusters
    vector<vector<NodeRandomWalkData>> clusts = clusters.second;
    for (const auto& cluster: clusts) {
        for (const auto& node: cluster) {
            size_t node_id = node.get_node_id();
            clustering_label[node_id] = cluster_id;
        }
        cluster_id++;
    }

    return clustering_label;
}

