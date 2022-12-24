//
// Created by jonathan on 24/12/2022.
//

#ifndef PRISM_MOTIF_H
#define PRISM_MOTIF_H

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "Concepts.h"
using namespace std;

using NodeId = size_t;
using NodeType = std::string;
using EdgeId = size_t;
using NodeName = std::string;
using Predicate = std::string;

class Motif : public HyperGraph {

private:
    NodeId source_concept;
public:
    Motif(Concept &abstract_concept, HyperGraph hypergraph);
};




#endif //PRISM_MOTIF_H
