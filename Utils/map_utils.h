#ifndef PRISM_MAP_UTILS_H
#define PRISM_MAP_UTILS_H

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

template<typename TK, typename TV>
void accumulateMaps(std::map<TK, TV>& map1, std::map<TK, TV>& map2) {
    for (const auto& [key, value] : map2) {
        if (map1.count(key) > 0) {
            map1[key] += value;
        } else {
            map1[key] = value;
        }
    }
}
#endif //PRISM_MAP_UTILS_H
