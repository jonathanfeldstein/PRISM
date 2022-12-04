#ifndef PRISM_TEST_RANDOM_WALKS_H
#define PRISM_TEST_RANDOM_WALKS_H

#include <string>
#include "HyperGraph.h"
#include "Concepts.h"
using namespace std;

void TestRandomWalks(string path_to_data);

void test_minimum_number_of_random_walks_for_truncated_hitting_times();

void test_minimum_number_of_random_walks_for_path_distribution();

void test_number_of_random_walks_ran();


#endif //PRISM_TEST_RANDOM_WALKS_H
