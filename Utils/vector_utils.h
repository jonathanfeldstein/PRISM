#ifndef PRISM_VECTOR_UTILS_H
#define PRISM_VECTOR_UTILS_H
#include <vector>
#include <set>
#include <numeric>
#include <algorithm>
#include <Eigen/Dense>


using namespace std;
using namespace Eigen;

vector<double> to_vector(VectorXd &v1);

template<typename T>
bool contain_same_elements(vector<T> first, vector<T> second){
    std::set<T> first_set;
    std::set<T> second_set;
    first_set.insert(first.begin(), first.end());
    second_set.insert(second.begin(), second.end());
    bool is_equal = (first_set == second_set);
    return is_equal;
}

template<typename T>
bool has(vector<T> checked_vector, T element){
    return find(checked_vector.begin(), checked_vector.end(), element) != checked_vector.end();
};

template<typename T>
vector<T> flatten(std::vector<std::vector<T>> const &vec)
{
    int size = 0;
    for (auto &v: vec) {
        size += v.size();
    }

    std::vector<T> flattened;
    flattened.reserve(size);

    for (auto &v: vec) {
        for (auto &e: v) {
            flattened.push_back(e);
        }
    }

    return flattened;
}

template <typename T>
vector<size_t> sort_indexes(const vector<T> &v, bool ascending) {

    // initialize original index locations
    vector<size_t> idx(v.size());
    iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    // using std::stable_sort instead of std::sort
    // to avoid unnecessary index re-orderings
    // when v contains elements of equal values
    if(ascending) {
        stable_sort(idx.begin(), idx.end(),
                    [&v](size_t i1, size_t i2) { return v[i1] < v[i2]; });
    }else{stable_sort(idx.begin(), idx.end(),
                      [&v](size_t i1, size_t i2) { return v[i1] > v[i2]; });}

    return idx;
}


template<typename T>
std::vector<int> find_indices_of_element(std::vector<T> const &v, int target) {
    std::vector<int> indices;
    auto it = v.begin();
    while ((it = std::find_if(it, v.end(), [&] (T const &e) { return e == target; }))
           != v.end())
    {
        indices.push_back(std::distance(v.begin(), it));
        it++;
    }
    return indices;
}

template<typename T>
std::set<T> to_set(std::vector<T> const &v){
    std::set<T> s(v.begin(), v.end());
    return s;
}



#endif //PRISM_VECTOR_UTILS_H
