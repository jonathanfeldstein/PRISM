//
// Created by jonathan on 24/12/2022.
//

#include "Motif.h"

Motif::Motif(Concept &abstract_concept, HyperGraph hypergraph) {
    set<EdgeId> added_edges{};
    this->source_concept = abstract_concept.source_node;
    // Creating a node for each cluster, i.e what we call concept in our paper TODO need to discuss with Dom nomenclature
    for (auto &node : abstract_concept.single_nodes) {
        this->nodes[node] = hypergraph.get_nodes()[node];
        this->node_types.insert(hypergraph.get_nodes()[node]);
    }
    for(auto cluster:abstract_concept.clusters){
        NodeId a_node_in_cluster = *cluster.begin();
        this->nodes[a_node_in_cluster] = hypergraph.get_nodes()[a_node_in_cluster];
        this->node_types.insert(hypergraph.get_nodes()[a_node_in_cluster]);

    }
    for (auto const &node:abstract_concept.nodes){
        for(auto original_edge : hypergraph.get_memberships(node)){
            if(!has(added_edges,original_edge)) {
                // Find the nodes that participate in the edge in the original hypergraph
                vector<NodeId> original_nodes = hypergraph.get_edge(original_edge);
                vector<NodeId> new_nodes{};
                // Verify that every node is also in one of the clusters.
                // If found, add the id of the concept that abstracts the node to new_nodes
                // Else (i.e. not all nodes are in the motif) drop that edge
                bool found_all_nodes = true;
                for(auto original_node:original_nodes){
                    if(has(abstract_concept.single_nodes, original_node)){
                        new_nodes.push_back(original_node); //We can just use the same node id, because the concepts have been named the same
                    }else {
                        NodeId new_node_id;
                        bool found_specific_node = false;
                        for(auto cluster:abstract_concept.clusters){
                            if(has(cluster, original_node)){
                                new_node_id = *cluster.begin(); // The concept ideas are based on the first node in each cluster
                                new_nodes.push_back(new_node_id);
                                found_specific_node = true;
                                break;
                            }
                        }
                        if(!found_specific_node){
                            found_all_nodes = false;
                        }
                    }
                }
                if(found_all_nodes){
                    string predicate = hypergraph.get_predicate(original_edge).data();
                    // Check if already exists and the weight just needs to be updated
                    bool added_edge = false;
                    vector<EdgeId> potential_similar_edges = this->get_memberships(new_nodes[0]);
                    for(auto potential_edge:potential_similar_edges){
                            vector<NodeId> nodes_in_edge = this->get_edge(potential_edge);
                            if(contain_same_elements(nodes_in_edge, new_nodes) &&
                               (predicate == this->get_predicate(potential_edge).data())){
                                this->edge_weights[potential_edge] += hypergraph.get_edge_weight(original_edge);
                                added_edge = true;
                                break;
                            }
                    }
                    // Completely new edges will be added
                    if(!added_edge){
                        double weight = hypergraph.get_edge_weight(original_edge);
                        this->predicate_argument_types[predicate] = hypergraph.get_predicate_argument_types()[predicate];
                        this->add_edge(original_edge, predicate, new_nodes, weight);
                    }
                }
                // We can add the edge to added edges so we don't need to test again even if it failed
                added_edges.insert(original_edge);
            }
        }
    }
}
