#ifndef PRISM_STATS_UTILS_H
#define PRISM_STATS_UTILS_H
#include <Eigen/Dense>
#include <boost/math/distributions/gamma.hpp>

using namespace std;
using namespace Eigen;
using namespace boost;

double estimate_generalised_chi_squared_critical_value_from_weight_vector(const VectorXd& weight_vector, double significance_level);
double estimate_generalised_chi_squared_critical_value_from_mean_and_variance(double mean, double variance, double significance_level);

#endif //PRISM_STATS_UTILS_H
