#include "HierarchicalClusterer.h"
#include "CommunityPrinter.h"
#include "Utils/profiling_utils.h"
#include "Utils/random_utils.h"
#include "read_files_utils.h"
#include "UnitTests/run_all_tests.h"
using namespace Eigen;

using namespace std;

int main(int argc, char** argv)
{
    if(argc >= 6){
        string db_file_path = argv[1];
        string info_file_path = argv[2];
        string output_filename = argv[3];
        bool multiprocessing = true;
        if(argc == 7){
            multiprocessing = stoi(argv[6]);
            if(multiprocessing == 0){
                cout << "NO MULTIPROCESSING" << endl;
            }else{
                cout << "MULTIPROCESSING" << endl;
            }
        }else{
            cout << "MULTIPROCESSING" << endl;
        }

        Timer timer("main");

        Timer timer_readfile("Read Files");
        HyperGraph original_hg(argv[1], argv[2], false);
        vector<HyperGraph> hgs = construct_hypergraphs_from_files(argv[1], argv[2], false);
        timer_readfile.Stop();

        Timer timer_hc("HC");
        HierarchicalClustererConfig config;
        config.max_lambda2 = stod(argv[7]);
        config.min_cluster_size = stoi(argv[8]);
        vector<HyperGraph> full_hc_clusters;
        for (auto hg: hgs) {
            if (hg.number_of_nodes() > 3) {
                HierarchicalClusterer hc(hg, config);
                vector<HyperGraph> hc_clusters = hc.run_hierarchical_clustering();
                full_hc_clusters.insert(full_hc_clusters.end(), hc_clusters.begin(), hc_clusters.end());
            } else{
//                full_hc_clusters.emplace_back(hg);
            }
        }
        timer_hc.Stop();

        Timer timer_rw("RW");
        RandomWalkerConfig config_rw{};
        config_rw.epsilon = stod(argv[4]);
        config_rw.alpha = stod(argv[5]);
        config_rw.multiprocessing = multiprocessing;

        vector<Communities> com_vector;
        for (auto HyperG: full_hc_clusters) {
            cout << "---------------------------------------------------------------------------------------\n";
            HyperG.print();
            Communities com = Communities(HyperG, config_rw);
            com.print();
            com_vector.emplace_back(com);
        }
        timer_rw.Stop();

        Timer timer_communities("Communities Printer");
        CommunityPrinter com_printer = CommunityPrinter(com_vector, original_hg);
        com_printer.write_files(output_filename);
        timer_communities.Stop();

        timer.Stop(); // Timing of full program
    }else{
        string path = argv[1];
        bool output = RunAllTests(path);
    }
    return 0;
}
