//
// Created by jonathan on 21/03/2022.
//

#ifndef FASTER_COMMUNITY_H
#define FASTER_COMMUNITY_H

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <list>

using namespace std;

struct Community{
    size_t source_node;
    set<size_t> single_nodes;
    vector<set<size_t>> clusters; // TODO CHeck whether it is vector of vector or vector of sets
    set<size_t> nodes_in_clusters;
    set<size_t> nodes;
    size_t number_of_clusters;
    size_t number_of_single_nodes;
    size_t number_of_nodes_in_clusters;
    size_t number_of_nodes;

    Community(size_t source_node, set<size_t> single_nodes, vector<set<size_t>> &clusters);
    ~Community();

    Community(const Community &) = default;
    Community(Community &&) = default;

    Community& operator=(const Community &) = default;
    Community& operator=(Community &&) = default;

    string print();

};
#endif //FASTER_COMMUNITY_H
