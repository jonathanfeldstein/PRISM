//
// Created by jonathan on 19/04/2022.
//

#include "stats_utils.h"

double estimate_generalised_chi_squared_critical_value_from_weight_vector(VectorXd weight_vector, double significance_level){

    // mean of generalised chi-squared distribution
    double mean_gen = weight_vector.sum();
    // variance of generalised chi-squared distribution
    double var_gen = 2 * weight_vector.dot(weight_vector);
    // estimated gamma scale parameter
    double scale = var_gen / mean_gen;
    // estimated gamma shape parameter
    double shape = mean_gen / scale;
    math::gamma_distribution gamma_distr(shape, scale);
    double critical_value = quantile(complement(gamma_distr, significance_level));

    return critical_value;
}

double estimate_generalised_chi_squared_critical_value_from_mean_and_variance(double mean, double variance, double significance_level){

    // estimated gamma scale parameter
    double scale = variance / mean;
    // estimated gamma shape parameter
    double shape = mean / scale;
    math::gamma_distribution gamma_distr(shape, scale);
    double critical_value = quantile(complement(gamma_distr, significance_level));

    return critical_value;
}