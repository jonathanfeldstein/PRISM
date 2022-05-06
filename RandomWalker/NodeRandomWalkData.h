//
// Created by jonathan on 21/03/2022.
//

#ifndef FASTER_NODERANDOMWALKDATA_H
#define FASTER_NODERANDOMWALKDATA_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "map_utils.h"

using namespace std;
using NodeId = size_t;
using NodeType = string;
using Path = string;


class NodeRandomWalkData{
private:
    NodeId node_id;
    NodeType node_type;
    map<Path, int> path_counts;
    size_t accumulated_hitting_time{0};
    size_t number_of_hits{0};
    double average_hitting_time{0};

public:
    NodeRandomWalkData();
    NodeRandomWalkData(NodeId node_id, NodeType node_type);
    NodeRandomWalkData(NodeId node_id, NodeType node_type, double hitting_time);
    ~NodeRandomWalkData();
    bool operator< (const NodeRandomWalkData &other) const;

    double get_average_hitting_time() const;

    void add_path(Path const& path);

    void update_number_of_hits();

    void update_accumulated_hitting_time(size_t hitting_time);

    void calculate_average_hitting_time(size_t number_of_walks, size_t max_length);

    map<Path, int> get_path_counts();

    NodeId get_node_id() const;

    NodeType get_node_type();

    size_t get_count_of_nth_path(size_t n);

    vector<pair<Path, int> > get_top_paths(size_t number_of_paths, size_t path_length);

};
#endif //FASTER_NODERANDOMWALKDATA_H
