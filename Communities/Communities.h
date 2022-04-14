//
// Created by jonathan on 21/03/2022.
//

#ifndef FASTER_COMMUNITIES_H
#define FASTER_COMMUNITIES_H

#include "HyperGraph.h"
#include "RandomWalker.h"
#include "Community.h"
#include "map_utils.h"
#include "clustering_utils.h"

class Communities{
private:
    HyperGraph hypergraph;
    RandomWalker random_walker;
    map<size_t, Community> communities;

    void check_arguments(); //TODO Shouldn't that be in RandomWalker?

public:
    Communities(HyperGraph hypergraph, RandomWalkerConfig config);
    ~Communities();

    void print();
    Community get_community(size_t &source_node, RandomWalkerConfig config);


};
#endif //FASTER_COMMUNITIES_H
