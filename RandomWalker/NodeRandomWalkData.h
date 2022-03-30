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
    string name;
    string node_type;
    map<string, int> path_counts;
    size_t accumulated_hitting_time{0};
    size_t number_of_hits{0};
    size_t average_hitting_time{0};

public:
    NodeRandomWalkData();
    NodeRandomWalkData(string node, string node_type);
    ~NodeRandomWalkData();

    void add_path(string const& path);

    void update_number_of_hits();

    void update_accumulated_hitting_time(double hitting_time);

    void calculate_average_hitting_time(size_t number_of_waks, size_t max_length);

    map<string, int> get_path_counts();

    string get_name();

    string get_node_type();

    size_t get_count_of_nth_path(size_t n);

    vector<pair<string, int> > get_top_paths(size_t number_of_paths, bool as_list);

};
#endif //FASTER_NODERANDOMWALKDATA_H
