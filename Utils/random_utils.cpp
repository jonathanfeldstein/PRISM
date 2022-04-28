//
// Created by jonathan on 13/04/2022.
//

#include "random_utils.h"

static unsigned int g_seed = 39428;

// Used to seed the generator.
inline void fast_srand(int seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer.

// Output value in range [0, 32767]

inline int fast_rand(void) {

    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;

}

// generates a uniform random integer in the range [0,max]
// (assuming max << 32767)
int uniform_random_int(int max){

    if (max == 0) {
        return 0;
    }  else {
        return fast_rand() % max;

    }

}
double uniform_random_double(double max){

    if (max == 0) {
        return 0;
    }  else {
        return (fast_rand()/32767.0) * max;

    }

}

int weighted_discrete_distribution(vector<double> weights){

    double sum =  accumulate(weights.begin(), weights.end(), 0.0);
    double random_number = uniform_random_double(sum);
    int chosen_number{0};
    while(true){
        random_number -= weights[chosen_number];
        if(random_number<=0){
            break;
        }else{
            chosen_number++;
        }
    }
    return chosen_number;
}