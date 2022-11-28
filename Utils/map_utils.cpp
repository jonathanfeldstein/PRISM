#include "map_utils.h"
using namespace std;

// Compare function to sort from highest to lowest
bool cmp(const pair<string, int> &a,
         const pair<string, int> &b)
{
    return a.second > b.second;
}

// Function to sort the map according
// to value in a (key-value) pairs vector
vector<pair<string, int> > sort(map<string, int>& M)
{

    // Declare vector of pairs
    vector<pair<string, int> > V;

    // Copy key-value pair from Map
    // to vector of pairs
    V.reserve(M.size());
    for (const auto& it : M) {
        V.emplace_back(it);
    }

    // Sort using comparator function
    sort(V.begin(), V.end(), cmp);

    return V;
}

// Function to sort the map according
// to value in a (key-value) pairs vector but normalized this leads to a probability
vector<pair<string, double> > normalize_and_sort_counts(map<string, int>& M, size_t normalization_constant)
{

    // Declare vector of pairs
    vector<pair<string, double> > V;

    // Copy key-value pair from Map
    // to vector of pairs
    V.reserve(M.size());
    for (auto& it : M) {
        V.emplace_back(it.first, it.second/(double)normalization_constant);
    }

    // Sort using comparator function
    sort(V.begin(), V.end());

    return V;
}