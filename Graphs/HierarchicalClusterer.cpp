//
// Created by jonathan on 21/03/2022.
//

#include "HierarchicalClusterer.h"


HierarchicalClusterer::HierarchicalClusterer(HyperGraph hypergraph, HierarchicalClustererConfig config) {
    if(config.min_cluster_size > 2 && 0 <= config.max_lambda2 && config.max_lambda2 <= 2){
        if(config.max_lambda2 == 0){
            cout << "WARNING: Max Lambda2 == 0 works but no hierarchical clustering will occur!" << endl;
        }
        if(config.max_lambda2 == 2){
            cout << "WARNING: Max Lambda2 == 2 works but hierarchical clustering will cut all the way to min cluster size!" << endl;
        }
        this->min_cluster_size = config.min_cluster_size;
        this->max_lambda2 = config.max_lambda2;
        this->hypergraph = hypergraph;
    }
    else{
        throw HC_InvalidArgumentException();
    }
}

HierarchicalClusterer::~HierarchicalClusterer() {

}

vector<HyperGraph> &HierarchicalClusterer::run_hierarchical_clustering() { //TODO CHeck that it correctly just returns address, make sure that it is const
    // 1. Convert hypergraph to graph
    UndirectedGraph original_graph(this->hypergraph);
    // 2. Hierarchical cluster the graph
    this->get_clusters(original_graph);
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
    return hypergraph_clusters;
}

void HierarchicalClusterer::get_clusters(UndirectedGraph &graph) {
    pair<VectorXd, double> second_eigenpair = graph.get_second_eigenpair();
    // stop splitting if lambda2 stop criterion met
    if(second_eigenpair.second > this->max_lambda2 || graph.number_of_nodes()< 2*this->min_cluster_size){
        this->graph_clusters.emplace_back(graph);
    }else{
        pair<UndirectedGraph, UndirectedGraph> subgraphs = std::move(graph.cheeger_cut(second_eigenpair.first));
        // stop splitting if cluster size stop criterion met
        if(this->min_cluster_size != 0 &&
        (subgraphs.first.number_of_nodes() < this->min_cluster_size ||
         subgraphs.second.number_of_nodes() < this->min_cluster_size)){
            this->graph_clusters.emplace_back(graph);
        }else{
            {
                this->get_clusters(subgraphs.first);
                this->get_clusters(subgraphs.second);
            }
        }

    }
}

vector<UndirectedGraph> HierarchicalClusterer::get_graph_clusters() {
    return this->graph_clusters;
}
