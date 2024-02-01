#ifndef PRISM_MAP_UTILS_H
#define PRISM_MAP_UTILS_H

#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "exceptions.h"
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

template<typename TK, typename TV>
static TV get_value(std::map<TK, TV> const& input_map, TK const&key) {
    try {
        TV value = input_map.at(key);
        return value;
    }
    catch (const std::out_of_range&) {
        throw KeyNotFoundException(key);
    }
}

bool cmp(const pair<string, int> &a,
         const pair<string, int> &b);

vector<pair<string, int> > sort(map<string, int>& M);

vector<pair<string, double> > normalize_and_sort_counts(map<string, int>& M, size_t normalization_constant);
#endif //PRISM_MAP_UTILS_H
