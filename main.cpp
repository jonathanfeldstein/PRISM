#include "HierarchicalClusterer.h"
#include "CommunityPrinter.h"
#include "Utils/profiling_utils.h"

using namespace Eigen;

using namespace std;


int main(int , char* [])
{

    Timer timer("main");
    HyperGraph hg("/home/jonathan/FASTER/Experiments/imdb1.db", "/home/jonathan/FASTER/Experiments/imdb1.info");

    HierarchicalClustererConfig config{};
    config.min_cluster_size = 3;
    config.max_lambda2 = 0.8;

    HierarchicalClusterer hc(hg, config);

    vector<HyperGraph> hc_clusters = hc.run_hierarchical_clustering();

    RandomWalkerConfig config_rw{};
    config_rw.epsilon = 0.1;
    config_rw.max_num_paths = 3;
    config_rw.max_path_length = 5;
    config_rw.theta_p = 0.001;
    config_rw.multiprocessing = true;

    vector<Communities> com_vector;

    for (auto HyperG: hc_clusters) {
//        cout << "---------------------------------------------------------------------------------------\n";
//        HyperG.print();
        Communities com = Communities(HyperG, config_rw);
//        com.print();
        com_vector.emplace_back(com);
    }


    CommunityPrinter com_printer = CommunityPrinter(com_vector, hg);
    string output_filename = "/home/jonathan/FASTER/Experiments/test_imdb";
    com_printer.write_files(output_filename);

// TODO: Random Walk Testing
//    RandomWalkerConfig rw_config{};
//    rw_config.epsilon = 0.05;
//    rw_config.max_num_paths = 3;
//    rw_config.alpha_sym = 0.1;
//    rw_config.pca_dim = 2;
//    rw_config.clustering_method_threshold = 50;
//    rw_config.max_path_length = 5;
//    rw_config.theta_p = 0.5;
//    rw_config.multiprocessing=false;
//
//    RandomWalker rw = RandomWalker(hc_clusters[5], rw_config);
//    map<size_t, NodeRandomWalkData> node_random_walk_data = rw.generate_node_random_walk_data(70);
//    for (auto data: node_random_walk_data) {
//        cout << endl;
//        cout << "Node Number" << endl;
//        cout << data.first;
//        data.second.print();
//    }




// TODO Graph Testing
//    bool is_connected();
//    bool check_is_source_node(int node_id);
//    void add_edge(size_t edge_id, string const& predicate, vector<size_t>node_ids);
//    void add_edge(string const& predicate, size_t node_id);
//    map<size_t, set<string>> get_singleton_edges();
//    set<size_t> get_node_ids();
//    map<size_t, vector<size_t>> get_edges();
//    map<size_t, string> get_nodes(); // TODO think about nomenclature
//    vector<size_t> get_nodes_of_edge(size_t edge_id);
//    map<size_t, string> get_predicates();
//    string get_predicate(size_t edge_id);
//    set<string> get_node_types();
//    map<size_t, set<size_t>> get_memberships();
//    set<size_t> get_memberships(size_t node_id);
//    size_t number_of_nodes(); //Number of nodes
//    size_t number_of_edges(); //Number of edges
//    int number_of_predicates();
//    double get_estimated_graph_diameter();
//    pair<size_t, size_t> get_random_edge_and_neighbor_of_node(size_t const& node);
//    map<string, vector<string>> get_predicate_argument_types();
//    vector<string> get_predicate_argument_types(string predicate);


// TODO: Random Walk Testing
//    RandomWalkerConfig rw_config{};
//    rw_config.epsilon = 0.05;
//    rw_config.max_num_paths = 3;
//    rw_config.alpha_sym = 0.1;
//    rw_config.pca_dim = 2;
//    rw_config.clustering_method_threshold = 50;
//    rw_config.max_path_length = 5;
//    rw_config.theta_p = 0.5;
//    rw_config.multiprocessing=false;
//
//    RandomWalker rw = RandomWalker(hc_clusters[5], rw_config);
//    map<size_t, NodeRandomWalkData> node_random_walk_data = rw.generate_node_random_walk_data(70);
//    for (auto data: node_random_walk_data) {
//        cout << endl;
//        cout << "Node Number" << endl;
//        cout << data.first;
//        data.second.print();
//    }




// TODO Graph Testing
//    bool is_connected();
//    bool check_is_source_node(int node_id);
//    void add_edge(size_t edge_id, string const& predicate, vector<size_t>node_ids);
//    void add_edge(string const& predicate, size_t node_id);
//    map<size_t, set<string>> get_singleton_edges();
//    set<size_t> get_node_ids();
//    map<size_t, vector<size_t>> get_edges();
//    map<size_t, string> get_nodes(); // TODO think about nomenclature
//    vector<size_t> get_nodes_of_edge(size_t edge_id);
//    map<size_t, string> get_predicates();
//    string get_predicate(size_t edge_id);
//    set<string> get_node_types();
//    map<size_t, set<size_t>> get_memberships();
//    set<size_t> get_memberships(size_t node_id);
//    size_t number_of_nodes(); //Number of nodes
//    size_t number_of_edges(); //Number of edges
//    int number_of_predicates();
//    double get_estimated_graph_diameter();
//    pair<size_t, size_t> get_random_edge_and_neighbor_of_node(size_t const& node);
//    map<string, vector<string>> get_predicate_argument_types();
//    vector<string> get_predicate_argument_types(string predicate);

    return 0;
}
