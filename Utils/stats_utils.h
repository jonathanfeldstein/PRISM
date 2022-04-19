//
// Created by jonathan on 19/04/2022.
//

#ifndef FASTER_STATS_UTILS_H
#define FASTER_STATS_UTILS_H
#include <Eigen/Dense>
#include <boost/math/distributions/non_central_chi_squared.hpp>
using namespace std;
using namespace Eigen;
using namespace boost;

double compute_non_central_chi_squared_critical_value(VectorXd lambdas, size_t degrees_of_freedom, double significance_level);

#endif //FASTER_STATS_UTILS_H
