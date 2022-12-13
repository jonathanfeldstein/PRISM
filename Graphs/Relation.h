#ifndef PRISM_RELATION_H
#define PRISM_RELATION_H

#include <vector>
#include <string>
#include <set>

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
#endif //PRISM_RELATION_H
