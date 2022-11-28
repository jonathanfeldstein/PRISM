//
// Created by jonathan on 03/05/2022.
//

#ifndef PRISM_RELATION_H
#define PRISM_RELATION_H

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
#endif //PRISM_RELATION_H
