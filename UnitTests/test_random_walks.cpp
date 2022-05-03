//
// Created by dominic on 01/05/22.
//

#include "test_random_walks.h"

void TestRandomWalks(string path_to_data) {
    string db_file = path_to_data + "/imdb1.db";
    string info_file = path_to_data + "/imdb1.info";
    HyperGraph H = HyperGraph(db_file, info_file);
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

    cout << "Random Walk Tests" << endl;
    if (walk_length > config.max_random_walk_length) {
        cout << "Random walks executed with a walk length of " << walk_length << ", whereas the max path length specified in the config was " << config.max_random_walk_length << endl;
    }
    cout << ".";
    if ((N_ran <= 0 || N_tht <= 0 || N_dist <= 0)) {
        cout << "The random walk calculation didn't work as expected: N_ran=" << N_ran << ", N_tht=" << N_tht << ", N_dist=" << N_dist << endl;
    }
    cout << ".";
    if (N_ran > max(N_dist, N_tht)) {
        cout << "Random walks executed more walks (" << N_ran << ") than the theoretical upper bound (" << max(N_dist, N_tht) << ")." << endl;
    }
    cout << ".";

    for (auto node: rw_data) {
        NodeRandomWalkData node_rw_data = node.second;
        if (node_rw_data.get_average_hitting_time() > walk_length) {
            cout << "Node had a larger average hitting time (" << node_rw_data.get_average_hitting_time() << ") than the length of the random walks (" << walk_length << ")." << endl;
        }
        vector<int> count_vector;
        map<string, int> path_counts = node_rw_data.get_path_counts();
        for (auto pair: path_counts) {
            count_vector.emplace_back(pair.second);
        }
        if (accumulate(count_vector.begin(), count_vector.end(), 0) > N_ran) {
            cout << "Node was hit more times than the number of random walks ran!";
        }
    }
    cout << ".";

}