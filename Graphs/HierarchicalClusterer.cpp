//
// Created by jonathan on 21/03/2022.
//

#include "HierarchicalClusterer.h"

//TODO Implement

HierarchicalClusterer::HierarchicalClusterer(HyperGraph hypergraph, HierarchicalClustererConfig config) {
    this->min_cluster_size = config.min_cluster_size;
    this->max_lambda2 = config.max_lambda2;
    this->hypergraph = hypergraph;
    // TODO add assertions
}

HierarchicalClusterer::~HierarchicalClusterer() {

}

vector<HyperGraph> &HierarchicalClusterer::run_hierarchical_clustering() { //TODO CHeck that it correctly just returns address, make sure that it is const
    Timer timer("HC Clustering");
    // 1. Convert hypergraph to graph
    UndirectedGraph original_graph(this->hypergraph);
//    omp_lock_t hc_support_lock;
//    omp_init_lock(&hc_support_lock);
    // 2. Hierarchical cluster the graph
    Timer clustertimer("clusters");
    this->get_clusters(original_graph);
    clustertimer.Stop();
    Timer subtimer("subtimer");
    omp_lock_t clusters_support_lock;
    omp_init_lock(&clusters_support_lock);
    // 3. Convert the graph clusters into hypergraphs
#pragma omp parallel for schedule(dynamic)
    for(size_t i=0; i<this->graph_clusters.size(); i++){
        auto &graph = this->graph_clusters[i];
        HyperGraph hypergraph_cluster_member(graph,this->hypergraph);
        omp_set_lock(&clusters_support_lock);
        this->hypergraph_clusters.emplace_back(std::move(hypergraph_cluster_member));
        omp_unset_lock(&clusters_support_lock);
    }
    subtimer.Stop();
    timer.Stop();
    return hypergraph_clusters;
}

void HierarchicalClusterer::get_clusters(UndirectedGraph &graph) {
    pair<VectorXd, double> second_eigenpair = graph.get_second_eigenpair();
    // stop splitting if lambda2 stop criterion met
    if(second_eigenpair.second > this->max_lambda2 || graph.number_of_nodes()< 2*this->min_cluster_size){
//        omp_set_lock(&hc_support_lock);
        this->graph_clusters.emplace_back(graph);
//        omp_unset_lock(&hc_support_lock);
        // TODO check recursiveness return NULL;
    }else{
        Timer hctimer("hctimer");
        pair<UndirectedGraph, UndirectedGraph> subgraphs = std::move(graph.cheeger_cut(second_eigenpair.first));
        hctimer.Stop();
        // stop splitting if cluster size stop criterion met
        if(this->min_cluster_size != 0 &&
        (subgraphs.first.number_of_nodes() < this->min_cluster_size ||
         subgraphs.second.number_of_nodes() < this->min_cluster_size)){
//            omp_set_lock(&hc_support_lock);
            this->graph_clusters.emplace_back(graph);
//            omp_unset_lock(&hc_support_lock);
            // TODO check recursiveness return NULL;
        }else{
//#pragma omp parallel num_threads(2)
            {
//#pragma omp task
                this->get_clusters(subgraphs.first);
//#pragma omp task
                this->get_clusters(subgraphs.second);
//#pragma omp taskwait
            }
        }

    }
}

vector<UndirectedGraph> HierarchicalClusterer::get_graph_clusters() {
    return this->graph_clusters;
}
