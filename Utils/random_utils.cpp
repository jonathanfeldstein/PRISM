//
// Created by jonathan on 13/04/2022.
//

#include "random_utils.h"

int uniform_random(int min, int max){
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> distr(min, max);
    return distr(rng);
}