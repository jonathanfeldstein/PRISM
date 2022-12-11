#ifndef PRISM_TEST_RANDOM_WALKS_H
#define PRISM_TEST_RANDOM_WALKS_H

#include <string>
#include "HyperGraph.h"
#include "Concepts.h"
#include "test_utils.h"
using namespace std;

TestCount TestRandomWalks(const string& path_to_data);

//TODO never implemented the following functions??

void test_minimum_number_of_random_walks_for_truncated_hitting_times();

void test_minimum_number_of_random_walks_for_path_distribution();

void test_number_of_random_walks_ran();


#endif //PRISM_TEST_RANDOM_WALKS_H
