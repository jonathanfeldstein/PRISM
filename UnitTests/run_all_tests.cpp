#include "run_all_tests.h"

void print_full_test_report(vector<string> test_names, vector<pair<size_t,size_t>> test_results){
    size_t MIN_LENGTH = 16;

    if(test_names.size() != test_results.size()){
        cout << "Number of test names and test results do not fit" <<endl;
    }else{
        // print out a line of "=====" as a top border
        for (int i = 0; i < test_names.size(); i++) {
            cout << "=";
            if (test_names[i].length() < MIN_LENGTH) {
                for(size_t j = 0; j < MIN_LENGTH; ++j){
                    cout << "=";
                }
            } else {
                for(size_t j = 0; j < test_names[i].length(); ++j){
                    cout << "=";
                }
            }
            if (i != test_names.size() - 1) {
                cout << "==";
            }else{
                cout << "=";
            }
        }
        cout << endl;
        // print out an empty line with separators
        for (int i = 0; i < test_names.size(); i++) {
            cout << " ";
            if (test_names[i].length() < MIN_LENGTH) {
                for(size_t j = 0; j < MIN_LENGTH; ++j){
                    cout << " ";
                }
            } else {
                for(size_t j = 0; j < test_names[i].length(); ++j){
                    cout << " ";
                }
            }
            if (i != test_names.size() - 1) {
                cout << " |";
            }else{
                cout << " ";
            }
        }
        cout << endl;
        // print test names
        for (int i = 0; i < test_names.size(); i++) {
            cout << " ";
            cout << test_names[i];
            if (test_names[i].length() < MIN_LENGTH) {
                for(size_t j = 0; j < MIN_LENGTH - test_names[i].length(); ++j){
                    cout << " ";
                }
            }
            if (i != test_names.size() - 1) {
                cout << " |";
            }else{
                cout << " ";
            }
        }
        cout << endl;
        // print total tests
        for (int i = 0; i < test_results.size(); i++) {
            cout << " Total Test:  ";
            if(test_results[i].first < 10){
                cout << " " << test_results[i].first;
            }else{
                cout << test_results[i].first;
            }
            if (test_names[i].length() > MIN_LENGTH) {
                for(size_t j = 0; j < test_names[i].length() - MIN_LENGTH; ++j){
                    cout << " ";
                }
            }
            if (i != test_names.size() - 1) {
                cout << "  |";
            }else{
                cout << " ";
            }
        }
        cout << endl;
        // print failed tests
        for (int i = 0; i < test_results.size(); i++) {
            cout << " Failed Test: ";
            if(test_results[i].second < 10){
                cout << " " << test_results[i].second;
            }else{
                cout << test_results[i].second;
            }
            if (test_names[i].length() > MIN_LENGTH) {
                for(size_t j = 0; j < test_names[i].length() - MIN_LENGTH; ++j){
                    cout << " ";
                }
            }
            if (i != test_names.size() - 1) {
                cout << "  |";
            }else{
                cout << " ";
            }
        }
        cout << endl;
        // print out an empty line with separators
        for (int i = 0; i < test_names.size(); i++) {
            cout << " ";
            if (test_names[i].length() < MIN_LENGTH) {
                for(size_t j = 0; j < MIN_LENGTH; ++j){
                    cout << " ";
                }
            } else {
                for(size_t j = 0; j < test_names[i].length(); ++j){
                    cout << " ";
                }
            }
            if (i != test_names.size() - 1) {
                cout << " |";
            }else{
                cout << " ";
            }
        }
        cout << endl;
        // print out a line of "=====" as a top border
        for (int i = 0; i < test_names.size(); i++) {
            cout << "=";
            if (test_names[i].length() < MIN_LENGTH) {
                for(size_t j = 0; j < MIN_LENGTH; ++j){
                    cout << "=";
                }
            } else {
                for(size_t j = 0; j < test_names[i].length(); ++j){
                    cout << "=";
                }
            }
            if (i != test_names.size() - 1) {
                cout << "==";
            }else{
                cout << "=";
            }
        }
        cout << endl;
    }
    cout << "Look above for details on the tests!" << endl;

}

bool RunAllTests(const string& path_to_data) {
    bool all_tests_passed = true;
    vector<pair<size_t,size_t>> tests;
    pair<size_t, size_t> test_count_hypergraph = TestHypergraph(path_to_data);
    tests.push_back(test_count_hypergraph);
    pair<size_t, size_t> test_count_undirected = TestUndirectedGraph(path_to_data);
    tests.push_back(test_count_undirected);
    pair<size_t, size_t> test_count_hierarchcial_clustering = TestHierarchicalClustering(path_to_data);
    tests.push_back(test_count_hierarchcial_clustering);
    pair<size_t, size_t> test_count_statistics = TestStatistics();
    tests.push_back(test_count_statistics);
    pair<size_t, size_t> test_count_random_walks = TestRandomWalks(path_to_data);
    tests.push_back(test_count_random_walks);
    pair<size_t, size_t> test_count_clustering = TestClustering();
    tests.push_back(test_count_clustering);

    print_full_test_report({"Hypergraph Tests",
                                      "Graph Tests",
                                      "Hierarchical Clustering Tests",
                                      "Statistics Tests",
                                      "Random Walk Tests",
                                      "Clustering Tests"},
                          tests);
    for(auto test:tests){
        if(test.second != 0){
            all_tests_passed = false;
        }
    }
    return  all_tests_passed;
}
