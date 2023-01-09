#include "test_random_walks.h"

pair<size_t, size_t> TestRandomWalks(const string& path_to_data) {
    cout << "------------------------------------------"<<endl;
    cout << "TESTING RANDOM WALKS" << endl;
    TestReport test_random_walk;
    string db_file = path_to_data + "/imdb1.db";
    string info_file = path_to_data + "/imdb1.info";
    HyperGraph H = HyperGraph(db_file, info_file, true);
    RandomWalkerConfig config;
    config.epsilon = 0.1;
    config.alpha = 0.01;
    config.num_top_paths_for_clustering = 3;
    config.max_random_walk_length = 5;
    config.multiprocessing = true;

    RandomWalker rw = RandomWalker(H, config);
    set<NodeId> node_id_set = H.get_node_ids();
    NodeId source_node = *(node_id_set.begin());
    map<size_t, NodeRandomWalkData> rw_data = rw.generate_node_random_walk_data(source_node);
    size_t walk_length = rw.get_length_of_walk();
    size_t N_tht = rw.number_of_walks_for_truncated_hitting_times;
    size_t N_dist = rw.number_of_walks_for_path_distribution;
    size_t N_ran = rw.get_number_of_walks_ran();

    if (walk_length > config.max_random_walk_length) {
        string message = "Random walks executed with a walk length of ";
        message += to_string(walk_length) + ", whereas the max path length specified in the config was "
                + to_string(config.max_random_walk_length) + "\n";
        test_random_walk.error_messages.push_back(message);
        test_random_walk.failed_tests++;
    }
    test_random_walk.total_tests++;

    if ((N_ran <= 0 || N_tht <= 0 || N_dist <= 0)) {
        string message = "The random walk calculation didn't work as expected: N_ran=";
        message += to_string(N_ran) + ", N_tht=" + to_string(N_tht) + ", N_dist=" + to_string(N_dist) + "\n";
        test_random_walk.error_messages.push_back(message);
        test_random_walk.failed_tests++;
    }
    test_random_walk.total_tests++;

    if (N_ran > max(N_dist, N_tht)) {
        string message = "Random walks executed more walks (";
        message += to_string(N_ran) + ") than the theoretical upper bound (" + to_string(max(N_dist, N_tht)) + ").\n";
        test_random_walk.error_messages.push_back(message);
        test_random_walk.failed_tests++;
    }
    test_random_walk.total_tests++;

    bool failed_average_hitting_time = false;
    bool failed_total_hits = false;

    for (const auto& node: rw_data) {
        NodeRandomWalkData node_rw_data = node.second;
        if (node_rw_data.get_average_hitting_time() > walk_length) {
            string message = "Node had a larger average hitting time (";
            message += to_string(node_rw_data.get_average_hitting_time())
                    + ") than the length of the random walks (" + to_string(walk_length) + ").\n";
            test_random_walk.error_messages.push_back(message);
            failed_average_hitting_time = true;
        }
        vector<int> count_vector;
        map<string, int> path_counts = node_rw_data.get_path_counts();
        for (const auto& pair: path_counts) {
            count_vector.emplace_back(pair.second);
        }
        if (accumulate(count_vector.begin(), count_vector.end(), 0) > N_ran) {
            string message = "Node ";
            message += to_string(node.first) + " was hit more times than the number of random walks ran!";
            test_random_walk.error_messages.push_back(message);
            failed_total_hits = true;
        }
    }
    if(failed_average_hitting_time){
        test_random_walk.failed_tests++;
    }
    if(failed_total_hits){
        test_random_walk.failed_tests++;
    }
    test_random_walk.total_tests += 2;

    // Check that starting a random walks from a periphery node has a chance of going back to hit the node
    bool failed_source_node = false;

    string db_file2 = path_to_data + "/smoking.db";
    string info_file2 = path_to_data + "/smoking.info";
    HyperGraph H2 = HyperGraph(db_file, info_file, true);
    RandomWalkerConfig config2;
    config.epsilon = 0.1;
    config.alpha = 0.01;
    config.num_top_paths_for_clustering = 3;
    config.max_random_walk_length = 5;
    config.multiprocessing = true;


    RandomWalker rw2 = RandomWalker(H2, config);
    map<size_t, NodeRandomWalkData> nrwd;
    // Generate empty random walk data for all nodes
    for (size_t node_id: H2.get_node_ids()) {
        nrwd[node_id] = NodeRandomWalkData();
    }
    set<NodeId> node_id_set2 = H2.get_node_ids();

    for (NodeId source_node2: node_id_set2) {
        // Run 1000 random walks starting from each node
        for (int i = 0; i < 1000; i++) {
            rw2.update_node_data_with_random_walk(source_node2, nrwd);
        }

        map<string, int> source_node_path_counts = nrwd[source_node2].get_path_counts();
        if (source_node_path_counts.empty()) {
            string message = "The source node ";
            message += to_string(source_node2)
                    + " was never hit! This might indicate that the random walk is not capable of "
                      "going backwards. However, if your datasets is extremely connected (Node degree O(1000)), "
                      "then this error can occur with ~30% probability for each node. Try this test on smoking.db "
                      "(default) in this dataset this error should not appear and indicates a mistake in the "
                      "random walk implementation.";
            test_random_walk.error_messages.push_back(message);
            failed_source_node = true;
        }

    }
    if(failed_source_node){
        test_random_walk.failed_tests++;
    }
    test_random_walk.total_tests++;

    // TEST DISTRIBUTION of RANDOM WALKS
    bool failed_on_distributions = false;
    string weighted_hypergraph_db = path_to_data + "/weighted_test.db";
    string weighted_hypergraph_info = path_to_data + "/weighted_test.info";
    HyperGraph weighted_hypergraph = HyperGraph(weighted_hypergraph_db, weighted_hypergraph_info, true);

    vector<size_t> hits(5, 0);
    int number_of_random_walks = 10000;
    for(int i{0}; i < number_of_random_walks; i++){
        pair<EdgeId, NodeId> edge_and_node = weighted_hypergraph.get_random_edge_and_neighbor_of_node(0);
        hits[edge_and_node.second]++;
    }
    vector<double> normalized_hits(5);

    for(int i{0}; i<5; i++){
        normalized_hits[i] = (double)hits[i]/number_of_random_walks;
    }

    vector<double> expected_distribution {0.0, 0.2083, 0.125, 0.4583, 0.2083};
    for (int i = 0; i < expected_distribution.size() ; i++) {
        if (abs(expected_distribution[i] - normalized_hits[i]) > 0.05 * expected_distribution[i]) {
            string message = "Expected distribution for the node ";
            message += to_string(i) + " is " + to_string(expected_distribution[i])
                    + " but the observed distribution was " + to_string(normalized_hits[i]) + "!\n";
            failed_on_distributions = true;
            test_random_walk.error_messages.push_back(message);
        }
    }
    if(failed_on_distributions){
        test_random_walk.failed_tests++;
    }
    test_random_walk.total_tests++;

    RandomWalkerConfig config3;
    config3.epsilon = 0.1;
    config3.alpha = 0.01;
    config3.num_top_paths_for_clustering = 3;
    config3.max_random_walk_length = 2;
    config3.multiprocessing = true;

    RandomWalker rw3 = RandomWalker(weighted_hypergraph, config3);
    // Hard code number of random walks to be 10000
    rw3.number_of_walks_for_path_distribution = 10000;
    rw3.number_of_walks_for_truncated_hitting_times = 10000;

    // Run random walks from source node 0
    map<size_t, NodeRandomWalkData> rw_data_from_node0 = rw3.generate_node_random_walk_data(0);

    // Check that the number of walks is correct
    int N = rw3.get_number_of_walks_ran();
    assert(N == 10000);

    // Check distribution of random walks
    map<Path, int> path_counts;
    for (const auto& rw_data_i:rw_data_from_node0){
        map<Path, int> new_path_counts = rw_data_i.second.get_path_counts();
        accumulateMaps(path_counts, new_path_counts);
    }
    vector<double> observed_distribution(5);

    vector<string> paths = {"E,E,", "E,P,", "E,ME,", "ME,E,", "ME,ME,"};
    for (int i = 0; i < paths.size(); i++) {
        Path p = Path(paths[i]);
        observed_distribution[i] = (double)path_counts[p]/N;
    }

    bool failed_on_path_distributions = false;
    vector<double> expected_distribution2 {2653.0/7020, 5.0/52, 26.0/135, 13.0/135, 32.0/135};
    for (int i = 0; i < expected_distribution2.size() ; i++) {
        if (abs(expected_distribution2[i] - observed_distribution[i]) > 0.05 * expected_distribution2[i]) {
            string message = "Expected distribution for the path ";
            message += paths[i] + " is " + to_string(expected_distribution2[i])
                    + " but the observed distribution was " + to_string(observed_distribution[i]) + "!\n";
            failed_on_path_distributions = true;
            test_random_walk.error_messages.push_back(message);
        }
    }
    if(failed_on_path_distributions){
        test_random_walk.failed_tests++;
    }
    test_random_walk.total_tests++;
    if(test_random_walk.failed_tests > 0){
        print_test_results("Random Walks", {test_random_walk});
    }
    return {test_random_walk.total_tests, test_random_walk.failed_tests};

}