#ifndef PRISM_COMMUNITY_H
#define PRISM_COMMUNITY_H

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <list>

using namespace std;
using NodeId = size_t;
using Cluster = set<NodeId>;

struct Community{
    NodeId source_node;
    set<NodeId> single_nodes;
    vector<Cluster> clusters;
    set<NodeId> nodes_in_clusters;
    set<NodeId> nodes;
    size_t number_of_clusters;
    size_t number_of_single_nodes;
    size_t number_of_nodes;

    Community(NodeId source_node, set<NodeId> single_nodes, vector<Cluster> &clusters);
    ~Community() = default;

    Community(const Community &) = default;
    Community(Community &&) = default;

    Community& operator=(const Community &) = default;
    Community& operator=(Community &&) = default;

    string print();

};
#endif //PRISM_COMMUNITY_H
