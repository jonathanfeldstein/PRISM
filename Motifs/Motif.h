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

    // Function to check whether a given set of nodes form a clique
    bool is_clique(std::set<NodeId> nodes);

    // Function to check whether two nodes are neighbors in the hypergraph
    bool are_neighbors(NodeId node1, NodeId node2);

    // Recursive function to find all cliques of a given size in the hypergraph
    std::vector<std::set<NodeId>> find_cliques_of_size(std::set<NodeId> current_clique, size_t clique_size);


public:
    Motif(Concept &abstract_concept, HyperGraph hypergraph);
    // Function to get the neighbors of a given node
    std::set<NodeId> get_neighbors(NodeId node);

    // Function to find all cliques in the hypergraph up to a maximum size
    // Recursive function to find all cliques in the hypergraph up to a maximum size
    // Function to find all cliques in the hypergraph up to a maximum size
    std::vector<std::set<NodeId>> find_cliques(size_t max_size);
};




#endif //PRISM_MOTIF_H
