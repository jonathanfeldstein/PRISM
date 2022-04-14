//
// Created by jonathan on 14/04/2022.
//

#ifndef FASTER_SET_UTILS_H
#define FASTER_SET_UTILS_H

#include <set>
#include <algorithm>

using namespace std;

template<typename T>
bool has(set<T> checked_set, T element){
    return find(checked_set.begin(), checked_set.end(), element) != checked_set.end();
};
#endif //FASTER_SET_UTILS_H
