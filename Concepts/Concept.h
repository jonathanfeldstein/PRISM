#ifndef PRISM_CONCEPT_H
#define PRISM_CONCEPT_H

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <list>

using namespace std;
using NodeId = size_t;
using Cluster = set<NodeId>;

struct Concept{
    NodeId source_node;
    set<NodeId> single_nodes;
    vector<Cluster> clusters;
    set<NodeId> nodes_in_clusters;
    set<NodeId> nodes;
    size_t number_of_clusters;
    size_t number_of_single_nodes;
    size_t number_of_nodes;

    Concept(NodeId source_node, set<NodeId> single_nodes, vector<Cluster> &clusters);
    ~Concept() = default;

    Concept(const Concept &) = default;
    Concept(Concept &&) = default;

    Concept& operator=(const Concept &) = default;
    Concept& operator=(Concept &&) = default;

    string print();

};
#endif //PRISM_CONCEPT_H
