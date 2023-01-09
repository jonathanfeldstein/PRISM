#include "test_statistics.h"

pair<size_t, size_t> TestStatistics() {

    pair<size_t, size_t> test_count{}; // Count of {total tests, failed tests}.
    TestReport gamma_function_test = test_gamma_function_approximation();
    vector<TestReport> sk_divergence_tests = test_sk_divergence_calculations();
    cout << "------------------------------------------"<<endl;
    cout << "TESTING STATISTICS" << endl;
    if(gamma_function_test.failed_tests >0){
        print_test_results("Gamma Function Approximation", {gamma_function_test});
    }
    size_t failed_tests = 0;
    for(const auto& test:sk_divergence_tests){
        failed_tests +=test.failed_tests;
    }
    if(failed_tests > 0){
        print_test_results("SK Divergence Calculations", sk_divergence_tests);
    }
    test_count.first += gamma_function_test.total_tests;
    test_count.second += gamma_function_test.failed_tests;
    for(const auto& test : sk_divergence_tests){
        test_count.first += test.total_tests;
        test_count.second += test.failed_tests;
    }
    return test_count;

}

TestReport test_gamma_function_approximation() {
    TestReport test_gamma_function;
    VectorXd weights1{5};
    weights1 << 1, 2, 3, 4, 5;
    VectorXd weights2{5};
    weights2 << 0.1, 0.2, 0.3, 0.4, 0.5;
    VectorXd weights3{5};
    weights3 << 1, 200, 30000, 4000000, 500000000;
    VectorXd weights4{12};
    weights4 << 0, 1, 0, 0, 3, 0, 6, 0, 12, 45, 0.4, 0;

    double c2 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights1, 0.5);
    double c1 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights1, 0.9999999);
    double c3 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights1, 0.0000001);
    if (!(c1 < c2 < c3)) {
        string message = "Error in generalised chi-squared critical value computation for weights1. Expected c1 < c2 < c3 but got c1=";
        message += to_string(c1) + " c2=" + to_string(c2) + " c3=" + to_string(c3) + ".\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;
    if (!((c1 > 0) && (c2 > 0) && (c3 > 0))){
        string message = "Error in generalised chi-squared critical value computation for weights1. Expected c1, c2, c3 >0 but got c1=";
        message += to_string(c1) + " c2=" + to_string(c2) + " c3=" + to_string(c3) + ".\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights2, 0.5);
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights2, 0.9999999);
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights2, 0.0000001);
    if (!(c1 < c2 < c3)) {
        string message = "Error in generalised chi-squared critical value computation for weights2. Expected c1 < c2 < c3 but got c1=";
        message += to_string(c1) + " c2=" + to_string(c2) + " c3=" + to_string(c3) + ".\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    if (!((c1 > 0) && (c2 > 0) && (c3 > 0))){
        string message = "Error in generalised chi-squared critical value computation for weights1. Expected c1, c2, c3 >0 but got c1=";
        message += to_string(c1) + " c2=" + to_string(c2) + " c3=" + to_string(c3) + ".\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights3, 0.5);
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights3, 0.9999999);
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights3, 0.0000001);
    if (!(c1 < c2 < c3)) {
        string message = "Error in generalised chi-squared critical value computation for weights3. Expected c1 < c2 < c3 but got c1=";
        message += to_string(c1) + " c2=" + to_string(c2) + " c3=" + to_string(c3) + ".\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    if (!((c1 > 0) && (c2 > 0) && (c3 > 0))){
        string message = "Error in generalised chi-squared critical value computation for weights1. Expected c1, c2, c3 >0 but got c1=";
        message += to_string(c1) + " c2=" + to_string(c2) + " c3=" + to_string(c3) + ".\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights4, 0.5);
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights4, 0.9999999);
    estimate_generalised_chi_squared_critical_value_from_weight_vector(weights4, 0.0000001);
    if (!(c1 < c2 < c3)) {
        string message = "Error in generalised chi-squared critical value computation for weights4. Expected c1 < c2 < c3 but got c1=";
        message += to_string(c1) + " c2=" + to_string(c2) + " c3=" + to_string(c3) + ".\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;
    if (!((c1 > 0) && (c2 > 0) && (c3 > 0))){
        string message = "Error in generalised chi-squared critical value computation for weights1. Expected c1, c2, c3 >0 but got c1=";
        message += to_string(c1) + " c2=" + to_string(c2) + " c3=" + to_string(c3) + ".\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

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

    //Alpha = 0.5 test
    double alpha1 = 0.5;
    double computed_critical1 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights, alpha1);
    double expected_critical1 = 0.000489575;
    if (abs(computed_critical1 - expected_critical1) > 0.01 * abs(computed_critical1 - expected_critical1)/expected_critical1) {
        string message = "Alpha = 0.5 test:\n";
        message += "Estimated critical value (" + to_string(computed_critical1)
                + ") and actual critical value (" + to_string(expected_critical1) + ") differ by more than 1%.\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    //Alpha = 0.1 test
    double alpha2 = 0.1;
    double computed_critical2 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights, alpha2);
    double expected_critical2 = 0.000976355;
    if (abs(computed_critical2 - expected_critical2) > 0.01 * abs(computed_critical2 - expected_critical2)/expected_critical2) {
        string message = "Alpha = 0.1 test:\n";
        message += "Estimated critical value (" + to_string(computed_critical2)
                   + ") and actual critical value (" + to_string(expected_critical2) + ") differ by more than 1%.\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    //Alpha = 0.01 test
    double alpha3 = 0.01;
    double computed_critical3 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights, alpha3);
    double expected_critical3 = 0.001545475;
    if (abs(computed_critical3 - expected_critical3) > 0.01 * abs(computed_critical3 - expected_critical3)/expected_critical3) {
        string message = "Alpha = 0.01 test:\n";
        message += "Estimated critical value (" + to_string(computed_critical3)
                   + ") and actual critical value (" + to_string(expected_critical3) + ") differ by more than 1%.\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    //Alpha = 0.001 test
    double alpha4 = 0.001;
    double computed_critical4 = estimate_generalised_chi_squared_critical_value_from_weight_vector(weights, alpha4);
    double expected_critical4 = 0.002064825;
    if (abs(computed_critical4 - expected_critical4) > 0.01 * abs(computed_critical4 - expected_critical4)/expected_critical4) {
        string message = "Alpha = 0.001 test:\n";
        message += "Estimated critical value (" + to_string(computed_critical4)
                   + ") and actual critical value (" + to_string(expected_critical4) + ") differ by more than 1%.\n";
        test_gamma_function.error_messages.push_back(message);
        test_gamma_function.failed_tests++;
    }
    test_gamma_function.total_tests++;

    return test_gamma_function;

}


vector<TestReport> test_sk_divergence_calculations() {

    vector<TestReport> test_results;

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


//    cout << endl << "Example 1" << endl;
    test_results.push_back(test_kl_divergence(p1, q1, kl_div_expected1));

    test_results.push_back(test_mean_distribution(p1, q1, m1));

    test_results.push_back(test_sk_divergence(p1, q1, sk_div_expected1));


//    cout << endl << "Example 2" << endl;
    test_results.push_back(test_kl_divergence(p2, q2, kl_div_expected2));

    test_results.push_back(test_mean_distribution(p2, q2, m2));

    test_results.push_back(test_sk_divergence(p2, q2, sk_div_expected2));


//    cout << endl << "Example 3" << endl;
    test_results.push_back(test_kl_divergence(p3, q3, kl_div_expected3));

    test_results.push_back(test_mean_distribution(p3, q3, m3));

    test_results.push_back(test_sk_divergence(p3, q3, sk_div_expected3));

    return test_results;
}

TestReport test_kl_divergence(map<string, double> p, map<string, double> q, double kl_expected) {

    TestReport test_kl;

    double kl_computed = kl_divergence(p, q);
    if (abs(kl_computed - kl_expected) > 0.000002) {
        string message = "Incorrect KL divergence calculation:\n";
        message += "Expected " + to_string(kl_expected) + " but observed " + to_string(kl_computed) + "\n";
        test_kl.error_messages.push_back(message);
        test_kl.failed_tests++;
    }
    test_kl.total_tests++;
    return test_kl;
}
TestReport test_mean_distribution(map<string, double> p, map<string, double> q, map<string, double> m_expected) {
    //Given two distributions p and q, test that the calculated average distribution is as expected
    TestReport test_mean;
    map<string, double> m_computed = compute_average_distribution(p, q);
    vector<string> m_computed_strings;
    vector<string> m_expected_strings;
    for (const auto& map_pair: m_computed) {
        m_computed_strings.emplace_back(map_pair.first);
    }
    for (const auto& map_pair: m_expected) {
        m_expected_strings.emplace_back(map_pair.first);
    }

    // For each path that appears in the computed mean distribution,
    // check that each computed probability matches expected probability
    bool failed_on_computed_strings = false;
    for (const string& m_string: m_computed_strings) {
        if (abs(m_computed[m_string] - m_expected[m_string]) > 0.000002) {
            string message = "Incorrect mean distribution value:\n";
            message += "For string " + m_string + " expected " + to_string(m_expected[m_string])
                    + " but observed " + to_string(m_computed[m_string]) + "\n";
            test_mean.error_messages.push_back(message);
            failed_on_computed_strings = true;
        }
    }
    if(failed_on_computed_strings){
        test_mean.failed_tests++;
    }
    test_mean.total_tests++;
    // As above, but check paths that appear in expected distribution
    // (i.e. ensures that there are no paths lost in computation)
    bool failed_on_expected_strings = false;
    for (const string& m_string: m_expected_strings) {
        if (abs(m_computed[m_string] - m_expected[m_string]) > 0.000002) {
            string message = "Incorrect mean distribution value:\n";
            message += "For string " + m_string + " expected " + to_string(m_expected[m_string])
                       + " but observed " + to_string(m_computed[m_string]) + "\n";
            test_mean.error_messages.push_back(message);
            failed_on_expected_strings = true;
        }
    }
    if(failed_on_expected_strings){
        test_mean.failed_tests++;
    }
    test_mean.total_tests++;

    return test_mean;
}

TestReport test_sk_divergence(map<string, double> p, map<string, double> q, double sk_expected) {
    TestReport test_sk;
    double sk_computed = sk_divergence(p, q);
    if (abs(sk_computed - sk_expected) > 0.000002) {
        string message = "Incorrect SK divergence calculation:\n";
        message += "Expected " + to_string(sk_expected) + " but observed " + to_string(sk_computed) + "\n";
        test_sk.error_messages.push_back(message);
        test_sk.failed_tests++;
    }
    test_sk.total_tests++;

    return test_sk;
}