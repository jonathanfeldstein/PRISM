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
    string source_node;
    set<string> single_nodes;
    vector<set<string>> clusters; // TODO CHeck whether it is vector of vector or vector of sets
    set<string> nodes_in_clusters;
    set<string> nodes;
    size_t number_of_clusters;
    size_t number_of_single_nodes;
    size_t number_of_nodes_in_clusters;
    size_t number_of_nodes;

    Community(string source_node, set<string> single_nodes, vector<set<string>> &clusters);
    ~Community();

    string print();

};
#endif //FASTER_COMMUNITY_H
