#include "HierarchicalClusterer.h"
#include "CommunityPrinter.h"
#include "Utils/profiling_utils.h"
#include "Utils/random_utils.h"
#include "Utils/pca_utils.h"
#include "UnitTests/run_all_tests.h"
#include "test_clustering.h"
using namespace Eigen;

using namespace std;

int main(int argc, char** argv)
{
    string path = "/home/dominic/CLionProjects/FASTER/Databases";
    bool output = RunAllTests(path);
    bool result = TestClustering();
//
//    Timer timer("main");
//    Timer readfile("readfile");
//    HyperGraph hg(argv[1], argv[2]);
//    readfile.Stop();
//    Timer timerhc("HC");
//    HierarchicalClustererConfig config{};
//    config.min_cluster_size = 3;
//    config.max_lambda2 = 0.8;
//
//    HierarchicalClusterer hc(hg, config);
//
//    vector<HyperGraph> hc_clusters = hc.run_hierarchical_clustering();
//    timerhc.Stop();
//    Timer timerrw("random walks");
//    RandomWalkerConfig config_rw{};
//    config_rw.epsilon = 0.1;
//    config_rw.num_top_paths_for_clustering = 3;
//    config_rw.max_random_walk_length = 5;
//    config_rw.alpha = 0.01;
//    config_rw.multiprocessing = true;
//
//    vector<Communities> com_vector;
//
//    for (auto HyperG: hc_clusters) {
//        cout << "---------------------------------------------------------------------------------------\n";
//        HyperG.print();
//        Communities com = Communities(HyperG, config_rw);
//        com.print();
//        com_vector.emplace_back(com);
//    }
//    timerrw.Stop();
//    Timer timercomms("Communities Printer");
//    CommunityPrinter com_printer = CommunityPrinter(com_vector, hg);
//    string output_filename = argv[3];
//    com_printer.write_files(output_filename);
//    timercomms.Stop();
//    timer.Stop();
    return 0;
}
