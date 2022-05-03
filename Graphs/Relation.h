//
// Created by jonathan on 03/05/2022.
//

#ifndef FASTER_RELATION_H
#define FASTER_RELATION_H

#include <vector>
#include <string>

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

#endif //FASTER_RELATION_H
