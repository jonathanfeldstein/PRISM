#include "test_statistics.h"

void TestStatistics() {

    cout << endl << "Testing Statistics" << endl;
    cout << "Testing Gamma Function Approximation" << endl;
    test_gamma_function_approximation();
    cout << endl << "Testing SK Divergence Calculations" << endl;
    test_sk_divergence_calculations();

}

void test_gamma_function_approximation() {

    VectorXd weights1{5};
    weights1 << 1, 2, 3, 4, 5;
    VectorXd weights2{5};
    weights2 << 0.1, 0.2, 0.3, 0.4, 0.5;
    VectorXd weights3{5};
    weights3 << 1, 200, 30000, 4000000, 500000000;
    VectorXd weights4{12};
    weights4 << 0, 1, 0, 0, 3, 0, 6, 0, 12, 45, 0.4, 0;

    double c2 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights1, 0.5);
    cout << ".";
    double c1 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights1, 0.9999999);
    cout << ".";
    double c3 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights1, 0.0000001);
    cout << ".";
    if (!(c1 < c2 < c3)) {
        cout << "Error in generalised chi-squared critical value computation for weights1. Expected c1 < c2 < c3 but got c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "." << endl;
    }
    cout << ".";
    if (!((c1 > 0) && (c2 > 0) && (c3 > 0))){
        cout << "Error in generalised chi-squared critical value computation for weights1. Expected c1, c2, c3 >0 but got c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "." << endl;
    }
    cout << ".";

    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights2, 0.5);
    cout << ".";
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights2, 0.9999999);
    cout << ".";
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights2, 0.0000001);
    cout << ".";
    if (!(c1 < c2 < c3)) {
        cout << "Error in generalised chi-squared critical value computation for weights2. Expected c1 < c2 < c3 but got c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "." << endl;
    }
    cout << ".";
    if (!((c1 > 0) && (c2 > 0) && (c3 > 0))){
        cout << "Error in generalised chi-squared critical value computation for weights1. Expected c1, c2, c3 >0 but got c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "." << endl;
    }
    cout << ".";

    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights3, 0.5);
    cout << ".";
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights3, 0.9999999);
    cout << ".";
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights3, 0.0000001);
    cout << ".";
    if (!(c1 < c2 < c3)) {
        cout << "Error in generalised chi-squared critical value computation for weights3. Expected c1 < c2 < c3 but got c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "." << endl;
    }
    cout << ".";
    if (!((c1 > 0) && (c2 > 0) && (c3 > 0))){
        cout << "Error in generalised chi-squared critical value computation for weights1. Expected c1, c2, c3 >0 but got c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "." << endl;
    }
    cout << ".";

    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights4, 0.5);
    cout << ".";
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights4, 0.9999999);
    cout << ".";
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights4, 0.0000001);
    cout << ".";
    if (!(c1 < c2 < c3)) {
        cout << "Error in generalised chi-squared critical value computation for weights4. Expected c1 < c2 < c3 but got c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "." << endl;
    }
    cout << ".";
    if (!((c1 > 0) && (c2 > 0) && (c3 > 0))){
        cout << "Error in generalised chi-squared critical value computation for weights1. Expected c1, c2, c3 >0 but got c1=" << c1 << " c2=" << c2 << " c3=" << c3 << "." << endl;
    }
    cout << ".";

    // Testing that the gamma approximation critical values are indeed good approximations for the generalised chi squared critical values
    VectorXd example_path_probabilities(6);
    example_path_probabilities << 0.2, 0.15, 0.07, 0.05, 0.02, 0.01;
    double number_of_pseudo_walks = 10000;
    VectorXd weights(6);
    size_t i = 0;
    for (auto prob: example_path_probabilities) {
        weights[i] = ((1/number_of_pseudo_walks)*(1-prob));
        i ++;
    }

    cout << endl << "Alpha = 0.5 test" << endl;
    double alpha1 = 0.5;
    double computed_critical1 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights, alpha1);
    double expected_critical1 = 0.000489575;
    if (abs(computed_critical1 - expected_critical1) > 0.01 * abs(computed_critical1 - expected_critical1)/expected_critical1) {
        cout << "Estimated critical value (" << computed_critical1 << ") and actual critical value (" << expected_critical1 << ") differ by more than 1%" << endl;
    }
    cout << "." << endl;

    cout << endl << "Alpha = 0.1 test" << endl;
    double alpha2 = 0.1;
    double computed_critical2 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights, alpha2);
    double expected_critical2 = 0.000976355;
    if (abs(computed_critical2 - expected_critical2) > 0.01 * abs(computed_critical2 - expected_critical2)/expected_critical2) {
        cout << "Estimated critical value (" << computed_critical2 << ") and actual critical value (" << expected_critical2 << ") differ by more than 1%" << endl;
    }
    cout << "." << endl;

    cout << endl << "Alpha = 0.01 test" << endl;
    double alpha3 = 0.01;
    double computed_critical3 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights, alpha3);
    double expected_critical3 = 0.001545475;
    if (abs(computed_critical3 - expected_critical3) > 0.01 * abs(computed_critical3 - expected_critical3)/expected_critical3) {
        cout << "Estimated critical value (" << computed_critical3 << ") and actual critical value (" << expected_critical3 << ") differ by more than 1%" << endl;
    }
    cout << "." << endl;

    cout << endl << "Alpha = 0.001 test" << endl;
    double alpha4 = 0.001;
    double computed_critical4 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights, alpha4);
    double expected_critical4 = 0.002064825;
    if (abs(computed_critical4 - expected_critical4) > 0.01 * abs(computed_critical4 - expected_critical4)/expected_critical4) {
        cout << "Estimated critical value (" << computed_critical4 << ") and actual critical value (" << expected_critical4 << ") differ by more than 1%" << endl;
    }
    cout << "." << endl;

}


void test_sk_divergence_calculations() {

    map<string, double> p1 = {{"path1", 0.5}, {"path2", 0.3}, {"path3", 0.1}};
    map<string, double> q1 = {{"path1", 0.4}, {"path2", 0.35}, {"path3", 0.15}};
    map<string, double> m1 = {{"path1", 0.45}, {"path2", 0.325}, {"path3", 0.125}};
    double kl_div_expected1 = 0.0247801;
    double sk_div_expected1 = 0.0251476;

    map<string, double> p2 = {{"path1", 0.5}, {"path2", 0.3}, {"path4", 0.1}};
    map<string, double> q2 = {{"path1", 0.4}, {"path2", 0.35}, {"path3", 0.15}};
    map<string, double> m2 = {{"path1", 0.45}, {"path2", 0.325}, {"path3", 0.075}, {"path4", 0.05}};
    double kl_div_expected2 = numeric_limits<double>::infinity();
    double sk_div_expected2 = numeric_limits<double>::infinity();

    map<string, double> p3 = {{"path1", 0.5}, {"path3", 0.3}, {"path4", 0.1}};
    map<string, double> q3 = {{"path2", 0.3}, {"path4", 0.35}, {"path5", 0.15}};
    map<string, double> m3 = {{"path1", 0.25}, {"path2", 0.15}, {"path3", 0.15}, {"path4", 0.225}, {"path5", 0.075}};
    double kl_div_expected3 = numeric_limits<double>::infinity();
    double sk_div_expected3 = numeric_limits<double>::infinity();

    cout << endl << "Example 1" << endl;
    test_kl_divergence(p1, q1, kl_div_expected1);
    cout << ".";
    test_mean_distribution(p1, q1, m1);
    cout << ".";
    test_sk_divergence(p1, q1, sk_div_expected1);
    cout << ".";

    cout << endl << "Example 2" << endl;
    test_kl_divergence(p2, q2, kl_div_expected2);
    cout << ".";
    test_mean_distribution(p2, q2, m2);
    cout << ".";
    test_sk_divergence(p2, q2, sk_div_expected2);
    cout << ".";

    cout << endl << "Example 3" << endl;
    test_kl_divergence(p3, q3, kl_div_expected3);
    cout << ".";
    test_mean_distribution(p3, q3, m3);
    cout << ".";
    test_sk_divergence(p3, q3, sk_div_expected3);
    cout << ".";
}

void test_kl_divergence(map<string, double> p, map<string, double> q, double kl_expected) {

    double kl_computed = kl_divergence(p, q);
    if (abs(kl_computed - kl_expected) > 0.000002) {
        cout << "Incorrect KL divergence calculation. Expected " << kl_expected << " but observed " << kl_computed << endl;
    }

    }

void test_mean_distribution(map<string, double> p, map<string, double> q, map<string, double> m_expected) {

    map<string, double> m_computed = compute_average_distribution(p, q);
    vector<string> m_computed_strings;
    vector<string> m_expected_strings;
    for (const auto& map_pair: m_computed) {
        m_computed_strings.emplace_back(map_pair.first);
    }
    for (const auto& map_pair: m_expected) {
        m_expected_strings.emplace_back(map_pair.first);
    }
    for (const string& m_string: m_computed_strings) {
        if (abs(m_computed[m_string] - m_expected[m_string]) > 0.000002) {
            cout << "Incorrect mean distribution value. For string " << m_string << " expected " << m_expected[m_string] << " but observed " << m_computed[m_string] << endl;
        }
    }
    for (const string& m_string: m_expected_strings) {
        if (abs(m_computed[m_string] - m_expected[m_string]) > 0.000002) {
            cout << "Incorrect mean distribution value. For string " << m_string << " expected " << m_expected[m_string] << " but observed " << m_computed[m_string] << endl;
        }
    }

    }

void test_sk_divergence(map<string, double> p, map<string, double> q, double sk_expected) {

    double sk_computed = sk_divergence(p, q);
    if (abs(sk_computed - sk_expected) > 0.000002) {
        cout << "Incorrect SK divergence calculation. Expected " << sk_expected << " but observed " << sk_computed << endl;
    }

    }