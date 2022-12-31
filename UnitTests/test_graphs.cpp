#include "test_graphs.h"

pair<size_t, size_t> TestHypergraph(const string& path_to_data){
    pair<size_t, size_t> test_count{}; // Count of {total tests, failed tests}.
    string smoking_db = path_to_data+"/smoking.db";
    string smoking_weighted_db = path_to_data+"/smoking_weighted.db";
    string smoking_info = path_to_data+"/smoking.info";

    string imdb_db = path_to_data+"/imdb1.db";
    string imdb_info = path_to_data+"/imdb1.info";

    string movie_lens_db = path_to_data+"/MovieLensMini.db";
    string movie_lens_info = path_to_data+"/MovieLensMini.info";

    HyperGraph small_hypergraph = HyperGraph(smoking_db, smoking_info, true);
    HyperGraph small_hypergraph_weighted = HyperGraph(smoking_weighted_db, smoking_info, true);
    HyperGraph medium_hypergraph = HyperGraph(imdb_db, imdb_info, true);
    HyperGraph large_hypergraph = HyperGraph(movie_lens_db, movie_lens_info, true);

    cout << "------------------------------------------"<<endl;
    cout << "TESTING HYPERGRAPHS" << endl << endl;
    int smoking_num_nodes = 8;
    int smoking_num_edges = 24;
    int smoking_num_predicates = 3;
    map<string, vector<string>> smoking_predicate_argument_types = {{"Friends",{"person", "person"}},
                                                                    {"Smokes", {"person"}},
                                                                    {"Cancer", {"person"}}};
    map<size_t, double> smoking_edge_weights = { {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
                                                 {6, 1}, {7, 1}, {8,1}, {9, 1}, {10, 1}, {11, 1},
                                                 {12, 1}, {13, 1}, {14, 1}, {15, 1}, {16, 1}, {17, 1},
                                                 {18, 1}, {19, 1}, {20, 1}, {21, 1}, { 22, 1}, {23, 1}};
    map<size_t, vector<size_t>> smoking_memberships = {{0, {0, 1, 2, 3, 4, 5, 14, 15, 18, 22}},
                                               {1, {0, 1, 6, 7, 16, 17}},
                                               {2, {2, 3, 10, 11, 19, 23}},
                                               {3, {4, 5, 10, 11, 20}},
                                               {4, {6, 7, 8, 9}},
                                               {5, {8, 9, 16, 17}},
                                               {6, {12, 13, 14, 15, 21}},
                                               {7, {12, 13}}};
    set<string> smoking_node_types = {"person"};

    // test unweighted small hypergraph
    TestReport test_small_reading_hypergraph = test_reading_hypergraph_from_database(small_hypergraph,
                                                                                     smoking_num_nodes,
                                                                                     smoking_num_edges,
                                                                                     smoking_predicate_argument_types,
                                                                                     smoking_edge_weights,
                                                                                     smoking_memberships,
                                                                                     smoking_node_types,
                                                                                     smoking_num_predicates);

    TestReport test_small_graph_conversion = test_graph_conversion(small_hypergraph);

    //Updating overall tests on hypergraph
    test_count.first += test_small_reading_hypergraph.total_tests;
    test_count.second += test_small_reading_hypergraph.failed_tests;
    test_count.first += test_small_graph_conversion.total_tests;
    test_count.second += test_small_graph_conversion.failed_tests;

    size_t failed_tests = 0;
    for(auto test: {test_small_reading_hypergraph, test_small_graph_conversion}){
        failed_tests += test.failed_tests;
    }
    if(failed_tests > 0) {
        print_test_results("Unweighted small hypergraph",
                           {test_small_reading_hypergraph, test_small_graph_conversion});
    }
    // test weighted small hypergraph
    map<size_t, double> smoking_edge_weights_2 = { {0, 0.4}, {1, 0.9}, {2, 1.0}, {3, 0.3}, {4, 0.001}, {5, 1},
                                                 {6, 1}, {7, 0}, {8,0.5}, {9, 0.25}, {10, 0.324}, {11, 0.3},
                                                 {12, 0.2}, {13, 0.3}, {14, 0.4}, {15, 0.5}, {16, 0.6}, {17, 0.7},
                                                 {18, 0.8}, {19, 0.9}, {20, 1}, {21, 0.11}, { 22, 0.12}, {23, 1}};
    TestReport test_weighted_reading_hypergraph = test_reading_hypergraph_from_database(small_hypergraph_weighted,
                                                                                        smoking_num_nodes,
                                                                                        smoking_num_edges,
                                                                                        smoking_predicate_argument_types,
                                                                                        smoking_edge_weights_2,
                                                                                        smoking_memberships,
                                                                                        smoking_node_types,
                                                                                        smoking_num_predicates);

    TestReport test_weighted_graph_conversion = test_graph_conversion(small_hypergraph_weighted);

    //Updating overall tests on hypergraph
    test_count.first += test_weighted_reading_hypergraph.total_tests;
    test_count.second += test_weighted_reading_hypergraph.failed_tests;
    test_count.first += test_weighted_graph_conversion.total_tests;
    test_count.second += test_weighted_graph_conversion.failed_tests;

    size_t failed_small_graph_tests = 0;
    for(auto test: {test_weighted_reading_hypergraph, test_weighted_graph_conversion}){
        failed_small_graph_tests += test.failed_tests;
    }
    if(failed_tests > 0) {
        print_test_results("Weighted small hypergraph",
                           {test_weighted_reading_hypergraph, test_weighted_graph_conversion});
    }

    // test medium hypergraph
    TestReport test_medium_graph_conversion = test_graph_conversion(medium_hypergraph);

    //Updating overall tests on hypergraph
    test_count.first += test_medium_graph_conversion.total_tests;
    test_count.second += test_medium_graph_conversion.failed_tests;

    if(test_medium_graph_conversion.failed_tests > 0) {
        print_test_results("Medium hypergraph", {test_medium_graph_conversion});
    }
    // test large hypergraph
    TestReport test_large_graph_conversion = test_graph_conversion(large_hypergraph);

    //Updating overall tests on hypergraph
    test_count.first += test_large_graph_conversion.total_tests;
    test_count.second += test_large_graph_conversion.failed_tests;
    if(test_large_graph_conversion.failed_tests >0) {
        print_test_results("Large hypergraph", {test_large_graph_conversion});
    }

    return test_count;
}

pair<size_t, size_t>  TestUndirectedGraph(const string& path_to_data) {

    // read from database
    string test_db = path_to_data+"/weighted_test.db";
    string test_info = path_to_data+"/weighted_test.info";
    HyperGraph H = HyperGraph(test_db, test_info, true);

    // create graph
    UndirectedGraph G = UndirectedGraph(H);

    MatrixXd test_adjacency(5,5);
    test_adjacency << 0, 0.5, 0.3, 1.5, 0.9, 0.5, 0.0, 0.0, 1.0, 0.0, 0.3, 0, 0, 0, 0, 1.5, 1.0, 0, 0, 0.8, 0.9, 0, 0, 0.8, 0;
    MatrixXd test_degree(5,5);
    test_degree << 3.2, 0, 0, 0, 0, 0, 1.5, 0, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 0, 3.3, 0, 0, 0, 0, 0, 1.7;
    MatrixXd test_laplacian(5,5);
    test_laplacian << 1, -0.228212, -0.30617, -0.461594, -0.385878, -0.228212, 1, 0, -0.449483, 0, -0.30617, 0, 1, 0, 0, -0.461594, -0.449483, 0., 1, -0.337787, -0.385878, 0., 0.,-0.337787, 1;
    MatrixXd test_sqrt_degree(5,5);
    test_sqrt_degree << 0.5590, 0, 0, 0, 0, 0, 0.8165, 0, 0, 0, 0, 0, 1.8257, 0, 0, 0, 0, 0, 0.5505, 0, 0, 0, 0, 0, 0.7670;
    VectorXd test_second_eigenvector(5);
    test_second_eigenvector << 0.300706, -0.498746, 0.745202, -0.289821, 0.146647;
    double test_second_eigenvalue = 0.876408;
    int test_diameter = 2;

    TestReport undirected_graph_tests =  test_undirected_graph(G,
                                                                 test_adjacency,
                                                                 test_degree,
                                                                 test_laplacian,
                                                                 test_sqrt_degree,
                                                                 test_second_eigenvector,
                                                                 test_second_eigenvalue,
                                                                 test_diameter);
    if(undirected_graph_tests.failed_tests > 0){
        print_test_results("Undirected Graph:", {undirected_graph_tests});
    }
    return {undirected_graph_tests.total_tests, undirected_graph_tests.failed_tests}; // Count of {total tests, failed tests}.
}

TestReport test_undirected_graph(UndirectedGraph &G,
                                 MatrixXd &Adjacency,
                                 MatrixXd &Degree,
                                 MatrixXd &Laplacian,
                                 MatrixXd &SqrtDegree,
                                 const VectorXd& second_eigenvector,
                                 double second_eigenvalue,
                                 int diameter){
    TestReport test_graph;
    // Test Adjacency Matrix
    if(G.get_adjacency_matrix().isApprox(Adjacency)){
        string message = "Adjacency matrix is not as expected:\n";
        message += "Expected:\n";
        message += toString(Adjacency) + "\n";
        message += "Actual:\n";
        message += toString(G.get_adjacency_matrix()) + "\n";
        test_graph.error_messages.push_back(message);
        test_graph.failed_tests++;
    }
    test_graph.total_tests++;
    // Test Degree Matrix
    if(G.get_degree_matrix().isApprox(Degree)){
        string message = "Degree matrix is not as expected.\n";
        message += "Expected:\n";
        message += toString(Degree) + "\n";
        message += "Actual:\n";
        message += toString(G.get_degree_matrix()) + "\n";
        test_graph.error_messages.push_back(message);
        test_graph.failed_tests++;
    }
    test_graph.total_tests++;
    // Test Laplacian
    if(G.get_laplacian_matrix().isApprox(Laplacian)){
        string message = "Laplacian matrix is not as expected.\n";
        message += "Expected:\n";
        message +=  toString(Laplacian) + "\n";
        message += "Actual:\n";
        message += toString(G.get_laplacian_matrix()) + "\n";
        test_graph.error_messages.push_back(message);
        test_graph.failed_tests++;
    }
    test_graph.total_tests++;
    // Test SqrtDegree
    if(G.get_sqrt_degree_matrix().isApprox(SqrtDegree)){
        string message = "Sqrt degree matrix is not as expected.\n";
        message += "Expected:\n";
        message += toString(SqrtDegree) + "\n";
        message += "Actual:\n";
        message +=  toString(G.get_sqrt_degree_matrix()) + "\n";
        test_graph.error_messages.push_back(message);
        test_graph.failed_tests++;
    }
    test_graph.total_tests++;
    // Test 2nd EigenPair
    auto second_eigenpair = G.get_second_eigenpair();
    if(second_eigenpair.first.isApprox(second_eigenvector)){
        string message = "Second EIGENVECTOR not as expected.\n";
        message += "Expected:\n";
        message += toString(second_eigenvector) + "\n";
        message += "Actual:\n";
        message += toString(second_eigenpair.first) + "\n";
        test_graph.error_messages.push_back(message);
        test_graph.failed_tests++;
    }
    test_graph.total_tests++;
    if(abs(second_eigenpair.second - second_eigenvalue) >= 0.02){
        string message = "Second Eigenvalue not as expected.\n";
        message += "Expected: " + to_string(second_eigenvalue) + " Actual: " + to_string(second_eigenpair.second) + "\n";
        test_graph.error_messages.push_back(message);
        test_graph.failed_tests++;
    }
    test_graph.total_tests++;
    // Test Diameter
    if(G.get_estimated_diameter() != diameter){
        string message = "DIAMETER not as expected.\n";
        message += "Expected: " + to_string(diameter) + " Actual: " + to_string(G.get_estimated_diameter()) + "\n";
        test_graph.error_messages.push_back(message);
        test_graph.failed_tests++;
    }
    test_graph.total_tests++;
    // Test Edges Not Needed tested through graph conversion function

    return test_graph;
}

TestReport test_reading_hypergraph_from_database(HyperGraph H1,
                                                 int number_of_nodes,
                                                 int number_of_edges,
                                                 const map<string, vector<string>>& predicate_argument_types,
                                                 map<size_t, double> edge_weights,
                                                 const map<size_t, vector<size_t>>& memberships,
                                                 const set<string>& node_types,
                                                 int number_of_predicates) {
    TestReport test_reading_hypergraph;
    if (H1.number_of_nodes() != number_of_nodes){
        string message = "Checking that the hypergraph has the expected number of nodes\n";
        message += "Expected #nodes: " +to_string(number_of_nodes) + " Actual " + to_string(H1.number_of_nodes()) + "\n";
        test_reading_hypergraph.error_messages.push_back(message);
        test_reading_hypergraph.failed_tests++;
    }
    test_reading_hypergraph.total_tests++;
    if(H1.number_of_edges() != number_of_edges){
        string message = "Checking that the hypergraph has the expected number of edges\n";
        message += "Expected #edges: " + to_string(number_of_edges) + " Actual " + to_string(H1.number_of_edges()) + "\n";
        test_reading_hypergraph.error_messages.push_back(message);
        test_reading_hypergraph.failed_tests++;
    }
    test_reading_hypergraph.total_tests++;
    bool failed_on_edge_weights = false;
    for(auto edge: get_keys(edge_weights)){
        if(H1.get_edge_weight(edge) != edge_weights[edge]){
            string message = "EDGE WEIGHT of EDGE ";
            message += to_string(edge) + " does not match expected weight\n";
            message += "Expected: " + to_string(edge_weights[edge]) + " Actual: " + to_string(H1.get_edge_weight(edge)) + "\n";
            test_reading_hypergraph.error_messages.push_back(message);
            failed_on_edge_weights = true;
        }
    }
    if(failed_on_edge_weights){
        test_reading_hypergraph.failed_tests++;
    }
    test_reading_hypergraph.total_tests++;
    // Check number of predicates
    set<string> predicates;
    for(auto edge: get_keys(H1.get_edges())){
        predicates.insert(H1.get_predicate(edge).data());
    }
    if(predicates.size() != number_of_predicates){
        string message = "NUMBER of PREDICATES does not match expected number\n";
        message += "Expected: " + to_string(number_of_predicates) + " Actual: " + to_string(predicates.size()) + "\n";
        test_reading_hypergraph.error_messages.push_back(message);
        test_reading_hypergraph.failed_tests++;
    }
    test_reading_hypergraph.total_tests++;
    // Check memberships
    if(H1.get_memberships() != memberships){
        string message = "MEMBERSHIPS do not match expected memberships\n";
        message += "Expected:\n";
        for (const auto& node_id : memberships) {
            message += to_string(node_id.first) + " : ";
            for (const auto& edge_ids : node_id.second) {
                message += to_string(edge_ids) += " ";
            }
        }
        message += "\n";
        message += "Actual:\n";
        for (const auto& node_id : H1.get_memberships()) {
            message += to_string(node_id.first) + " : ";
            for (const auto& edge_ids : node_id.second) {
                message += to_string(edge_ids) + " ";
            }
        }
        message += "\n";
        test_reading_hypergraph.error_messages.push_back(message);
        test_reading_hypergraph.failed_tests++;
    }
    test_reading_hypergraph.total_tests++;
    if(H1.get_predicate_argument_types() != predicate_argument_types){
        string message = "Predicate argument types map does not match the expected map\n";
        message += "Expected:\n";
        for (const auto& pred : predicate_argument_types) {
            message += pred.first + " : ";
            for (const auto& argument_type : pred.second) {
                message += argument_type + " ";
            }
        }
        message += "\n";
        message += "Actual:\n";
        for (const auto& pred : H1.get_predicate_argument_types()) {
            message += pred.first + " : ";
            for (const auto& argument_type : pred.second) {
                message += argument_type + " ";
            }
        }
        message += "\n";
        test_reading_hypergraph.error_messages.push_back(message);
        test_reading_hypergraph.failed_tests++;
    }
    test_reading_hypergraph.total_tests++;
    if(H1.get_node_types() != node_types){
        string message = "Node types do not match the expected node types.\n";
        message += "Expected:\n";
        for (const auto& node_type : node_types) {
            message += node_type + " ";
        }
        message += "\n";
        message += "Actual:\n";
        for (const auto& node_type: H1.get_node_types()) {
            message += node_type + " ";
        }
        test_reading_hypergraph.error_messages.push_back(message);
        test_reading_hypergraph.failed_tests++;
    }
    test_reading_hypergraph.total_tests++;

    return test_reading_hypergraph;
}

TestReport test_graph_conversion(HyperGraph &H1) {
    /*
     * Test that a HyperGraph->Graph-Hypergraph conversion preserves the original hypergraph.
     * This should show that the conversion to and from graphs is correct.
     */
    UndirectedGraph G = UndirectedGraph(H1);
    TestReport test_conversion;
    if (G.number_of_nodes() != H1.number_of_nodes()){
        string message = "Checking number of nodes in the graph is the same as the hypergraph\n";
        message += "Expected #nodes: " + to_string(H1.number_of_nodes()) + "\n";
        message += "Actual " + to_string(G.number_of_nodes()) + "\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;

    HyperGraph H2(G, H1);

    // number of nodes
    if(H1.number_of_nodes() != H2.number_of_nodes()){
        string message = "Number of NODES of original hypergraph and reconstructed hypergraph are not the same.\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    // number of edges
    if(H1.number_of_edges() != H2.number_of_edges()){
        string message = "Number of EDGES of original hypergraph and reconstructed hypergraph are not the same.\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    // map<size_t, vector<size_t>> edges; // edge_id : list(node_id)

    if(H1.get_edges() != H2.get_edges()){
        string message = "EDGES of original hypergraph and reconstructed hypergraph are not the same.\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    // map<size_t, double> edge_weights; // edge_id : weight
    bool failed_edge_weight = false;
    for(auto edge: get_keys(H1.get_edges())){
        if(H1.get_edge_weight(edge) != H2.get_edge_weight(edge)){
            string message = "WEIGHT of EDGE ";
            message += to_string(edge) + " of original hypergraph and reconstructed hypergraph are not the same.\n";
            test_conversion.error_messages.push_back(message);
            failed_edge_weight = true;
        }
    }
    if(failed_edge_weight){
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    // map<size_t, string> predicates; // edge_id : predicate_name
    bool failed_predicates = false;
    for(auto edge: get_keys(H1.get_edges())){
        if(H1.get_predicate(edge) != H2.get_predicate(edge)){
            string message = "PREDICATE of EDGE ";
            message += to_string(edge) + " of original hypergraph and reconstructed hypergraph are not the same.\n";
            test_conversion.error_messages.push_back(message);
            failed_predicates = true;
        }
    }
    if(failed_predicates){
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    //map<size_t, string> node_ids_names; // node_id : node_name
    if(H1.get_node_ids_names() != H2.get_node_ids_names()) {
        string message = "The MAPPING of Node IDS -> NAMES of original hypergraph and reconstructed hypergraph are not the same.\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    //map<string, size_t> node_names_ids; // node_name : node_id
    if(H1.get_node_names_ids() != H2.get_node_names_ids()){
        string message = "Node names ids of original hypergraph and reconstructed hypergraph are not the same.\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    //map<size_t, string> nodes; // node_id : node_type
    if(H1.get_nodes() != H2.get_nodes()){
        string message = "NODES of original hypergraph and reconstructed hypergraph are not the same.\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    //map<size_t, vector<size_t>> memberships; // node_id : set(edge_id)
    bool failed_memberships = false;
    for(auto memberships_h1: H1.get_memberships()){
        sort(memberships_h1.second.begin(), memberships_h1.second.end());
        auto memberships_h2 = H2.get_memberships(memberships_h1.first);
        sort(memberships_h2.begin(), memberships_h2.end());
        if(memberships_h1.second != memberships_h2){
            string message = "MEMBERSHIPS of original hypergraph and reconstructed hypergraph are not the same.\n";
            message += "For EDGE: " + to_string(memberships_h1.first) + "\n";
            message += "H1: ";
            for(auto edge: memberships_h1.second){
                message += to_string(edge) + " ";
            }
            message += "\n";
            message += "H2: ";
            for(auto edge: memberships_h2){
                message += to_string(edge) + " ";
            }
            message += "\n";
            failed_memberships = true;
            test_conversion.error_messages.push_back(message);
        }
    }
    if(failed_memberships){
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;

    //map<string, vector<string>> predicate_argument_types; // predicate_name : list(node_type)
    if(H1.get_predicate_argument_types() != H2.get_predicate_argument_types()){
        string message = "PREDICATE ARGUMENT TYPES of original hypergraph and reconstructed hypergraph are not the same.\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;
    //set<string> node_types; // set(node_type)
    if(H1.get_node_types() != H2.get_node_types()){
        string message = "NODE TYPES of original hypergraph and reconstructed hypergraph are not the same.\n";
        test_conversion.error_messages.push_back(message);
        test_conversion.failed_tests++;
    }
    test_conversion.total_tests++;

    return test_conversion;
}

