//
// Created by jonathan on 19/04/2022.
//

#ifndef FASTER_STATS_UTILS_H
#define FASTER_STATS_UTILS_H
#include <Eigen/Dense>
#include <boost/math/distributions/gamma.hpp>

using namespace std;
using namespace Eigen;
using namespace boost;

double estimate_generalised_chi_squared_critical_value(VectorXd weight_vector, double significance_level);

#endif //FASTER_STATS_UTILS_H
