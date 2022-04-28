//
// Created by jonathan on 13/04/2022.
//

#ifndef FASTER_RANDOM_UTILS_H
#define FASTER_RANDOM_UTILS_H
#include <random>
using namespace std;

int uniform_random_int(int max);
double uniform_random_double(double max);
int weighted_discrete_distribution(vector<double> weights);
#endif //FASTER_RANDOM_UTILS_H
