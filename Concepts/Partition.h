//
// Created by dominic on 13/12/22.
//

#ifndef PRISM_PARTITION_H
#define PRISM_PARTITION_H

#include "NodeRandomWalkData.h"

using namespace std;
using NodeId = size_t;
using Cluster = set<NodeId>;
using RandomWalkCluster = vector<NodeRandomWalkData>;

struct NodePartition{
    set<size_t> single_nodes{}; // Nodes that have not been clustered -> They form their individual singleton clusters
    vector<Cluster> clusters{}; // Cluster of nodes
};

struct RandomWalkNodePartition : public NodePartition { // A node partition with information about the random walks that led to this partition
    set<NodeId> single_nodes; // Nodes that have not been clustered -> They form their individual singleton clusters
    vector<RandomWalkCluster> clusters; //Clusters of nodes with information about the path count data
};

#endif //PRISM_PARTITION_H
