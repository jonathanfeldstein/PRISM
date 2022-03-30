//
// Created by jonathan on 21/03/2022.
//

#ifndef FASTER_COMMUNITIES_H
#define FASTER_COMMUNITIES_H

#include "HyperGraph.h"
#include "RandomWalker.h"
#include "Community.h"
#include "map_utils.h"


class Communities{
private:
    HyperGraph hypergraph;
    RandomWalker random_walker;
    map<string, Community> communities;

    void check_arguments(); //TODO Shouldn't that be in RandomWalker?

public:
    Communities(HyperGraph hypergraph, RandomWalkerConfig config);
    ~Communities();

    void print();
    Community get_community(string &source_node, RandomWalkerConfig config);


};
#endif //FASTER_COMMUNITIES_H
