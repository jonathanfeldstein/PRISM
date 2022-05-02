#include "HierarchicalClusterer.h"
#include "CommunityPrinter.h"
#include "Utils/profiling_utils.h"
#include "Utils/random_utils.h"
#include "Utils/pca_utils.h"
#include "UnitTests/run_all_tests.h"
using namespace Eigen;

using namespace std;

//#include <Eigen/Core>
//#include <Spectra/SymEigsSolver.h>
//#include <iostream>
//using namespace Spectra;
//
//// M = diag(1, 2, ..., 10)
//class MyDiagonalTen
//{
//public:
//    using Scalar = double;  // A typedef named "Scalar" is required
//    int rows() const { return 10; }
//    int cols() const { return 10; }
//    // y_out = M * x_in
//    void perform_op(const double *x_in, double *y_out) const
//    {
//        for(int i = 0; i < rows(); i++)
//        {
//            y_out[i] = x_in[i] * (i + 1);
//        }
//    }
//};

//int main()
//{
//    MyDiagonalTen op;
//    SymEigsSolver<MyDiagonalTen> eigs(op, 3, 6);
//    eigs.init();
//    eigs.compute(SortRule::LargestAlge);
//    if(eigs.info() == CompInfo::Successful)
//    {
//        Eigen::VectorXd evalues = eigs.eigenvalues();
//        std::cout << "Eigenvalues found:\n" << evalues << std::endl;
//    }
//
//    return 0;
//}

int main(int argc, char** argv)
{
//    string path = "/home/dominic/CLionProjects/FASTER/Databases";
//    bool output = RunAllTests(path);
    Timer timer("main");
    Timer readfile("readfile");
    HyperGraph hg(argv[1], argv[2]);
    readfile.Stop();
    Timer timerhc("HC");
    HierarchicalClustererConfig config{};
    config.min_cluster_size = 3;
    config.max_lambda2 = 0.8;

    HierarchicalClusterer hc(hg, config);

    vector<HyperGraph> hc_clusters = hc.run_hierarchical_clustering();
    timerhc.Stop();
    RandomWalkerConfig config_rw{};
    config_rw.epsilon = 0.1;
    config_rw.max_num_paths = 3;
    config_rw.max_path_length = 5;
    config_rw.theta_p = 0.01;
    config_rw.multiprocessing = true;

    vector<Communities> com_vector;

    for (auto HyperG: hc_clusters) {
        cout << "---------------------------------------------------------------------------------------\n";
        HyperG.print();
        Communities com = Communities(HyperG, config_rw);
        com.print();
        com_vector.emplace_back(com);
    }


    CommunityPrinter com_printer = CommunityPrinter(com_vector, hg);
    string output_filename = argv[3];
    com_printer.write_files(output_filename);

//// TODO: Random Walk Testing
////    RandomWalkerConfig rw_config{};
////    rw_config.epsilon = 0.05;
////    rw_config.max_num_paths = 3;
////    rw_config.alpha_sym = 0.1;
////    rw_config.pca_dim = 2;
////    rw_config.clustering_method_threshold = 50;
////    rw_config.max_path_length = 5;
////    rw_config.theta_p = 0.5;
////    rw_config.multiprocessing=false;
////
////    RandomWalker rw = RandomWalker(hc_clusters[5], rw_config);
////    map<size_t, NodeRandomWalkData> node_random_walk_data = rw.generate_node_random_walk_data(70);
////    for (auto data: node_random_walk_data) {
////        cout << endl;
////        cout << "Node Number" << endl;
////        cout << data.first;
////        data.second.print();
////    }
//
//
//
//
//// TODO Graph Testing
////    bool is_connected();
////    bool check_is_source_node(int node_id);
//
    timer.Stop();
    return 0;
}
