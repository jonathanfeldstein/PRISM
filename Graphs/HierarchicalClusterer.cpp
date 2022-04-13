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

    // 1. Convert hypergraph to graph
    UndirectedGraph original_graph(this->hypergraph);

    // 2. Hierarchical cluster the graph
    this->get_clusters(original_graph);

    // 3. Convert the graph clusters into hypergraphs
    for(UndirectedGraph &graph: this->graph_clusters){
        this->hypergraph_clusters.emplace_back(graph,this->hypergraph);
    }
    return hypergraph_clusters;
}

void HierarchicalClusterer::get_clusters(UndirectedGraph &graph) {
    pair<VectorXd, double> second_eigenpair = graph.get_second_eigenpair();

    // stop splitting if lambda2 stop criterion met
    if(second_eigenpair.second > this->max_lambda2 || graph.number_of_nodes()< 2*this->min_cluster_size){
        this->graph_clusters.push_back(graph);
        // TODO check recursiveness return NULL;
    }else{
        pair<UndirectedGraph, UndirectedGraph> subgraphs = graph.cheeger_cut(second_eigenpair.first);
        // stop splitting if cluster size stop criterion met
        if(this->min_cluster_size != 0 &&
        (subgraphs.first.number_of_nodes() < this->min_cluster_size ||
         subgraphs.second.number_of_nodes() < this->min_cluster_size)){
            this->graph_clusters.push_back(graph);
            // TODO check recursiveness return NULL;
        }else{
            this->get_clusters(subgraphs.first);
            this->get_clusters(subgraphs.second);
        }

    }
}
