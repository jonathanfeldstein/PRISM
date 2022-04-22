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

//TODO Move to utils
bool cmp(const pair<string, int>& a,
         const pair<string, int>& b);

class NodeRandomWalkData{
private:
    size_t node_id;
    string node_type;
    map<string, int> path_counts;
    size_t accumulated_hitting_time{0};
    size_t number_of_hits{0};
    double average_hitting_time{0};

public:
    NodeRandomWalkData();
    NodeRandomWalkData(size_t node_id, string node_type);
    ~NodeRandomWalkData();
    bool operator< (const NodeRandomWalkData &other) const;

    double get_average_hitting_time() const;

    void add_path(string const& path);

    void update_number_of_hits();

    void update_accumulated_hitting_time(size_t hitting_time);

    void calculate_average_hitting_time(size_t number_of_walks, size_t max_length);

    map<string, int> get_path_counts();

    size_t get_node_id() const;

    string get_node_type();

    size_t get_count_of_nth_path(size_t n);

    vector<pair<string, int> > get_top_paths(size_t number_of_paths);

};
#endif //FASTER_NODERANDOMWALKDATA_H
