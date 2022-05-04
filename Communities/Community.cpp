//
// Created by jonathan on 21/03/2022.
//

#include "Community.h"

#include <utility>

Community::Community(NodeId source_node, set<NodeId> single_nodes, vector<Cluster> &clusters) {
    this->source_node = source_node;
    this->single_nodes = std::move(single_nodes);
    this->clusters = clusters;
    for(auto cluster: clusters){
        this->nodes_in_clusters.merge(cluster); //TODO Check that it does not delete the previous clusters.
    }
    this->nodes = this->single_nodes;
    set<size_t> nodes_in_clusters_copy = this->nodes_in_clusters;
    this->nodes.merge(nodes_in_clusters_copy);

    this->number_of_clusters = this->clusters.size();
    this->number_of_single_nodes = this->single_nodes.size();
    this->number_of_nodes = this->nodes.size();

}

string Community::print() {
    string source_str = "SOURCE: " + to_string(this->source_node) +"\n ---------------------------- \n";
    string single_nodes_str;
    for(auto node: this->single_nodes){
        single_nodes_str += "SINGLE: " + to_string(node) + "\n";
    }
    string clusters_str;
    for(size_t cluster_id{0}; cluster_id < this->clusters.size(); cluster_id++){
        clusters_str += "CLUSTER " + to_string(cluster_id) + ": \n";
        for(auto node: this->clusters[cluster_id]){
            clusters_str += "\t\t" + to_string(node) + "\n";
        }
    }
    return source_str+single_nodes_str+clusters_str + "\n";
}


