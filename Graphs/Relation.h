//
// Created by jonathan on 03/05/2022.
//

#ifndef FASTER_RELATION_H
#define FASTER_RELATION_H

#include <vector>
#include <string>
#include <set>

using NodeId = std::size_t;
using Cluster = std::set<NodeId>;
using Predicate = std::string;

struct GroundRelation{
    Predicate predicate;
    std::vector<std::string> arguments;
    double weight;
};

struct Relation{
    Predicate predicate;
    std::vector<std::string> arguments;
};

struct NodePartition{
    std::set<NodeId> single_nodes;
    std::vector<Cluster> clusters;
};
#endif //FASTER_RELATION_H
