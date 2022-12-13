//
// Created by dominic on 13/12/22.
//

#ifndef PRISM_PARTITION_H
#define PRISM_PARTITION_H

struct NodePartition{
    std::set<NodeId> single_nodes;
    std::vector<Cluster> clusters;
};

union GeneralisedNodePartition{

};
#endif //PRISM_PARTITION_H
