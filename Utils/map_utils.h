//
// Created by jonathan on 19/03/2022.
//

#ifndef FASTER_MAP_UTILS_H
#define FASTER_MAP_UTILS_H

#include <map>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

template<typename TK, typename TV>
static set<TK> get_keys(map<TK, TV> const& input_map) {
    set<TK> retval;
    for (auto const& element : input_map) {
        retval.insert(element.first);
    }
    return retval;
}

template<typename TK, typename TV>
static vector<TV> get_values(std::map<TK, TV> const& input_map) {
    vector<TV> retval;
    for (auto const& element : input_map) {
        retval.push_back(element.second);
    }
    return retval;
}

bool cmp(const pair<string, int> &a,
         const pair<string, int> &b);

vector<pair<string, int> > sort(map<string, int>& M);

vector<pair<string, double> > normalize_and_sort_counts(map<string, int>& M, size_t normalization_constant);
#endif //FASTER_MAP_UTILS_H
