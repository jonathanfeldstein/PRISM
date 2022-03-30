//
// Created by jonathan on 21/03/2022.
//

#include "Community.h"

Community::Community(string source_node, set<string> single_nodes, vector<set<string>> &clusters) {
    this->source_node = source_node;
    this->single_nodes = single_nodes;
    this->clusters = clusters;
    for(set<string> cluster: clusters){
        this->nodes_in_clusters.merge(cluster); //TODO Check that it does not delete the previous clusters.
    }
    this->nodes = this->single_nodes;
    this->nodes.merge(this->nodes_in_clusters); //TODO this certainly DOES NOT create a copy ->Needs fixing

    this->number_of_clusters = this->clusters.size();
    this->number_of_single_nodes = this->single_nodes.size();
    this->number_of_nodes_in_clusters = this->nodes_in_clusters.size();
    this->number_of_nodes = this->nodes.size();

}

Community::~Community() {

}

string Community::print() {
    string source_str = "SOURCE: " + this->source_node+"\n ---------------------------- \n";
    string single_nodes_str;
    for(string const& node: this->single_nodes){ //TODO Check if that works with const
        single_nodes_str += "SINGLE: " + node + "\n";
    }
    string clusters_str;
    for(size_t cluster_id{0}; cluster_id < this->clusters.size(); cluster_id++){
        clusters_str += "CLUSTER " + to_string(cluster_id) + ": \n";
        for(string node: this->clusters[cluster_id]){
            clusters_str += "\t\t" + node + "\n";
        }
    }
    return source_str+single_nodes_str+clusters_str + "\n";
}


