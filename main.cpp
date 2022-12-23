#include "HierarchicalClusterer.h"
#include "ConceptPrinter.h"
#include "Utils/profiling_utils.h"
#include "Utils/random_utils.h"
#include "read_files_utils.h"
#include "UnitTests/run_all_tests.h"
using namespace Eigen;

using namespace std;

void check_number_of_arguments(int argc) {
    if(argc < 6) {
        throw ArgumentsException("Program failed: Not enough arguments.");
    } else if (argc > 9) {
        throw ArgumentsException("Program failed: Too many arguments.");
    };
}

bool set_bool_argument(int argc, char** argv, int arg_index, const string& arg_name, bool default_value) {
    bool arg;
    if(argc > arg_index) {
        arg = stoi(argv[arg_index]);
        if (arg == 0) {
            cout << arg_name << " will not be used." << endl;
        } else {
            cout << arg_name << " will be used." << endl;
        }
    } else {
        arg = default_value;
        cout << arg_name << " will be used. (default)" << endl;
    }

    return arg;
}

int main(int argc, char** argv)
{
    if(argc == 2) {
        // Run tests
        string path_to_test_data = argv[1];
        RunAllTests(path_to_test_data);
        return 0;
    } else {
        // Run the program
        check_number_of_arguments(argc);
        string db_file_path = argv[1];
        string info_file_path = argv[2];
        string output_filename = argv[3];
        double epsilon = stod(argv[4]);
        double alpha = stod(argv[5]);

        bool multiprocessing = set_bool_argument(argc, argv, 6, "multiprocessing", true);
        bool verbose = set_bool_argument(argc, argv, 7, "verbose", false);
        bool safe = set_bool_argument(argc, argv, 8, "safe", true);

        Timer timer("main");
        Timer timer_readfile("Read Files");

        // Load the entire database as a single hypergraph
        HyperGraph original_hg(db_file_path, info_file_path, safe);
        // Load each hypergraph in the data, note that the data might not be fully connected
        // In case the entire data is connected, the function returns a vector with a single hypergraph
        // Otherwise, the function returns a vector with a hypergraph for each connected component
        vector<HyperGraph> hgs = construct_hypergraphs_from_files(db_file_path, info_file_path, safe);

        timer_readfile.Stop();
        if(verbose) {
            original_hg.print();
        }

        double max_lambda2;
        int min_cluster_size;
        // Check if lambda2 and min_cluster_size specified
        if(argc == 9) {
            throw ArgumentsException("Program failed: If max_lambda2 specified, then min_cluster_size must also be specified.");
        } else if(argc == 10) {
            max_lambda2 = stod(argv[9]);
            min_cluster_size = stoi(argv[10]);
        } else {
            // If not specified, set defaults
            max_lambda2 = 0.7;
            min_cluster_size = 4;
        }

        Timer timer_hc("Hierarchical Clustering");
        HierarchicalClustererConfig config;
        vector<HyperGraph> full_hc_clusters;
        config.max_lambda2 = max_lambda2;
        config.min_cluster_size = min_cluster_size;

        // Run hierarchical clustering on each connected component
        for (auto hg: hgs) {
            if (hg.number_of_nodes() > min_cluster_size) {
                HierarchicalClusterer hc(hg, config);
                vector<HyperGraph> hc_clusters = hc.run_hierarchical_clustering();
                full_hc_clusters.insert(full_hc_clusters.end(), hc_clusters.begin(), hc_clusters.end());
            } else{
                // Add that component to the hypergraph clusters
                full_hc_clusters.emplace_back(hg);
            }
        }
        timer_hc.Stop();

        Timer timer_rw("Random Walks");
        RandomWalkerConfig config_rw{};
        config_rw.epsilon = epsilon;
        config_rw.alpha = alpha;
        config_rw.multiprocessing = multiprocessing;

        vector<Concepts> com_vector;
        // Learn concepts in each hypergraph cluster
        for (auto HyperG: full_hc_clusters) {
            if(verbose){
                cout << "---------------------------------------------------------------------------------------\n";
                HyperG.print();
            }
            Concepts com = Concepts(HyperG, config_rw);
            if(verbose){
                com.print();
            }
            com_vector.emplace_back(com);
        }
        timer_rw.Stop();


        // Print Concepts to a file
        Timer timer_concepts("Concepts Printer");
        ConceptPrinter com_printer = ConceptPrinter(com_vector, original_hg);
        com_printer.write_files(output_filename);
        timer_concepts.Stop();

        timer.Stop();
    }
    return 0;
}
