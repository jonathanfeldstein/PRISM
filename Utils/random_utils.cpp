//
// Created by jonathan on 13/04/2022.
//

#include "random_utils.h"

int uniform_random(int min, int max, mt19937 rng){
    uniform_int_distribution<int> distr(min, max);
    return distr(rng);
}