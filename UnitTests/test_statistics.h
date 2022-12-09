#ifndef PRISM_TEST_STATISTICS_H
#define PRISM_TEST_STATISTICS_H

#include "sk_divergence_utils.h"
#include "stats_utils.h"
#include <map>
#include <iostream>
#include "TestCount.h"

using namespace std;

void TestStatistics();

TestCount test_gamma_function_approximation();

void test_sk_divergence_calculations();

void test_kl_divergence(map<string, double> p, map<string, double> q, double kl_expected, TestCount test_count);

void test_mean_distribution(map<string, double> p, map<string, double> q, map<string,double> m_expected, TestCount test_count);

void test_sk_divergence(map<string, double> p, map<string, double> q, double sk_expected, TestCount test_count);


#endif //PRISM_TEST_STATISTICS_H
