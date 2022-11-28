#ifndef PRISM_RANDOM_UTILS_H
#define PRISM_RANDOM_UTILS_H
#include <random>
using namespace std;

int uniform_random_int(size_t max);
double uniform_random_double(double max);
int weighted_discrete_distribution(vector<double> weights);
#endif //PRISM_RANDOM_UTILS_H
