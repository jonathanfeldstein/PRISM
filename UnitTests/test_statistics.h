#ifndef PRISM_TEST_STATISTICS_H
#define PRISM_TEST_STATISTICS_H

#include "sk_divergence_utils.h"
#include "stats_utils.h"
#include <map>
#include <iostream>
#include "test_utils.h"

using namespace std;

pair<size_t, size_t> TestStatistics();

TestReport test_gamma_function_approximation();

vector<TestReport> test_sk_divergence_calculations();

TestReport test_kl_divergence(map<string, double> p, map<string, double> q, double kl_expected);

TestReport test_mean_distribution(map<string, double> p, map<string, double> q, map<string,double> m_expected);

TestReport test_sk_divergence(map<string, double> p, map<string, double> q, double sk_expected);


#endif //PRISM_TEST_STATISTICS_H
