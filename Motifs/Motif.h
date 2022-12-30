//
// Created by jonathan on 24/12/2022.
//

#ifndef PRISM_MOTIF_H
#define PRISM_MOTIF_H

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <omp.h>
#include "Concepts.h"
using namespace std;

using NodeId = size_t;
using NodeType = std::string;
using EdgeId = size_t;
using NodeName = std::string;
using Predicate = std::string;

class Motif : public HyperGraph {

private:
    NodeId source_concept;

    // is_clique_bron_kerbosch: uses the Bron–Kerbosch algorithm to check if a given set of nodes forms a clique
    bool is_clique_bron_kerbosch(std::set<NodeId> combination) {
        // check if the combination is empty
        if (combination.empty()) {
            return true;
        }

        // iterate over all pairs of nodes in the combination
        for (auto it1 = combination.begin(); it1 != combination.end(); ++it1) {
            for (auto it2 = std::next(it1); it2 != combination.end(); ++it2) {
                // check if the two nodes are not connected by an edge
                auto memberships1 = get_memberships(*it1);
                auto memberships2 = get_memberships(*it2);
                std::sort(memberships1.begin(), memberships1.end());
                std::sort(memberships2.begin(), memberships2.end());
                std::set<EdgeId> intersection;
                std::set_intersection(memberships1.begin(), memberships1.end(), memberships2.begin(), memberships2.end(),
                                      std::inserter(intersection, intersection.begin()));
                if (intersection.empty()) {
                    // if the two nodes are not connected by an edge, the combination is not a clique
                    return false;
                }
            }
        }

        // if every pair of nodes in the combination is connected by an edge, the combination is a clique
        return true;
    }


// generate_node_combinations: uses the algorithm described in Knuth (1997) to generate all the node combinations of size up to max_size
    std::vector<std::set<NodeId>> generate_node_combinations(int max_size) {
        std::vector<std::set<NodeId>> combinations;

        // get the set of all node IDs in the hypergraph
        std::set<NodeId> nodes = get_node_ids();

        // iterate over the sizes of the combinations
        for (int size = 1; size <= max_size; size++) {
            // initialize a vector to store the current combinations of the given size
            std::vector<std::set<NodeId>> current_combinations;

            // handle the base case
            if (size == 1) {
                for (auto& node : nodes) {
                    combinations.push_back({node});
                }
            } else {
                // iterate over the nodes in the hypergraph
                for (auto& node : nodes) {
                    // iterate over the combinations of the previous size
                    for (auto& prev_combination : combinations) {
                        // check if the current node is not in the previous combination
                        if (!prev_combination.count(node)) {
                            // if the current node is not in the previous combination,
                            // create a new combination by adding the current node to the previous combination
                            std::set<NodeId> current_combination = prev_combination;
                            current_combination.insert(node);

                            // add the combination to the current combinations vector
                            current_combinations.push_back(current_combination);
                        }
                    }
                }

                // add the current combinations to the overall combinations vector
                combinations.insert(combinations.end(), current_combinations.begin(), current_combinations.end());
            }
        }

        return combinations;
    }

public:
    Motif(Concept &abstract_concept, HyperGraph hypergraph);



    std::map<std::set<NodeId>, std::set<EdgeId>> find_cliques_optimized(int max_size) {
        // initialize a map to store the cliques
        std::map<std::set<NodeId>, std::set<EdgeId>> cliques;

        // use the algorithm described in Knuth (1997) to generate all the node combinations of size up to max_size
        std::vector<std::set<NodeId>> combinations = generate_node_combinations(max_size);

        // iterate over the combinations
        for (auto combination : combinations) {
            // check if the combination is a clique
            if (is_clique_bron_kerbosch(combination)) {
                // if the combination is a clique, add it to the cliques map
                std::set<EdgeId> edges;
                for (auto& node : combination) {
                    for (auto& edge : get_memberships(node)) {
                        // check if all the nodes in the edge are a subset of the combination
                        auto edge_members = get_edge(edge);
                        std::sort(edge_members.begin(), edge_members.end());
                        if (std::includes(combination.begin(), combination.end(), edge_members.begin(), edge_members.end())) {
                            edges.insert(edge);
                        }
                    }
                }
                if(!edges.empty()){
                    cliques[combination] = edges;
                }
            }
        }

        return cliques;
    }
};




#endif //PRISM_MOTIF_H
