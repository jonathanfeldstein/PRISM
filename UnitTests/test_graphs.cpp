//
// Created by dominic on 01/05/22.
//

#include "test_graphs.h"

bool TestHypergraph(){
    bool state = true;
    string smoking_db = "./Databases/smoking.db";
    string smoking_weighted_db = "./Databases/smoking_weighted.db";
    string smoking_info = "./Databases/smoking.info";

    string imdb_db = "./Databases/imdb1.db";
    string imdb_info = "./Databases/imdb1.info";

    string movie_lens_db = "./Databases/MoveLensMini.db";
    string movie_lens_info = "./Databases/MovieLensMini.info";

    HyperGraph small_hypergraph = HyperGraph(smoking_db, smoking_info);
    HyperGraph small_hypergraph_weighted = HyperGraph(smoking_weighted_db, smoking_info);
    HyperGraph medium_hypergraph = HyperGraph(imdb_db, imdb_info);
    HyperGraph large_hypergraph = HyperGraph(movie_lens_db, movie_lens_info);

    cout << "Testing Hypergraphs" << endl;
    int smoking_num_nodes = 8;
    int smoking_num_edges = 22;
    int smoking_num_singleton_edges = 6;
    int smoking_num_predicates = 3;
    map<string, vector<string>> smoking_predicate_argument_types = {{"Friends",
                                                                               {"person", "person"}},
                                                                    {"Smokes", {"person"}},
                                                                    {"Cancer", {"person"}}};
    map<size_t, double> smoking_edge_weights = { {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1},
                                                 {6, 1}, {7, 1}, {8,-1}, {9, 1}, {10, 1}, {11, 1},
                                                 {12, 1}, {13, 1}, {14, 1}, {15, 1}, {16, 1}, {17, 1},
                                                 {18, 1}, {19, 1}, {20, 1}, {21, 1}, { 22, 1}, {23, 1}};
    map<size_t, vector<size_t>> smoking_memberships = {{0, {0, 1, 2, 3, 4, 5, 14, 15}},
                                               {1, {0, 1, 6, 7, 16, 17}},
                                               {2, {2, 3, 10, 11}},
                                               {3, {4, 5, 10, 11}},
                                               {4, {6, 7, 8, 9}},
                                               {5, {8, 9, 16}},
                                               {6, {12, 13, 15}},
                                               {7, {12, 13}}};
    set<string> smoking_node_types = {"person"};

    // test unweighted small hypergraph
    test_reading_hypergraph_from_database(small_hypergraph,
                                          smoking_num_nodes,
                                          smoking_num_edges,
                                          smoking_num_singleton_edges,
                                          smoking_predicate_argument_types,
                                          smoking_edge_weights,
                                          smoking_memberships,
                                          smoking_node_types,
                                          smoking_num_predicates);
    test_graph_conversion(small_hypergraph);

    // test weighted small hypergraph
    map<size_t, double> smoking_edge_weights_2 = { {0, 0.4}, {1, 0.9}, {2, 1.3}, {3, 3}, {4, 0.001}, {5, 1},
                                                 {6, 1}, {7, 0}, {8,-1}, {9, 2.5}, {10, 3.24}, {11, 3},
                                                 {12, 2}, {13, 3}, {14, 4}, {15, 5}, {16, 6}, {17, 7},
                                                 {18, 8}, {19, 9}, {20, 10}, {21, 11}, { 22, 12}, {23, 13}};
    test_reading_hypergraph_from_database(small_hypergraph_weighted,
                                          smoking_num_nodes,
                                          smoking_num_edges,
                                          smoking_num_singleton_edges,
                                          smoking_predicate_argument_types,
                                          smoking_edge_weights_2,
                                          smoking_memberships,
                                          smoking_node_types,
                                          smoking_num_predicates);
    if(!test_graph_conversion(small_hypergraph_weighted)){
        state = false;
        cout << "FAILED in test_graph_conversion" << endl;
    }

    // test medium hypergraph
    if(!test_graph_conversion(medium_hypergraph)){
        state = false;
        cout << "FAILED in test_graph_conversion" << endl;
    }

    // test large hypergraph
    if(!test_graph_conversion(large_hypergraph)){
        state = false;
        cout << "FAILED in test_graph_conversion" << endl;
    }
    return state;
}

bool TestUndirectedGraph() {

    // read from database
    string test_db = "./Databases/weighted_test.db";
    string test_info = "./Databases/weighted_test.info";
    HyperGraph H = HyperGraph(test_db, test_info);

    // create graph
    UndirectedGraph G = UndirectedGraph(H);

    MatrixXd test_adjacency(5,5);
    test_adjacency << 0, 0.5, 0.3, 1.5, 0.9, 0.5, 0.0, 0.0, 1.0, 0.0, 0.3, 0, 0, 0, 0, 1.5, 1.0, 0, 0, 0.8, 0.9, 0, 0, 0.8, 0;
    MatrixXd test_degree(5,5);
    test_degree << 3.2, 0, 0, 0, 0, 0, 1.5, 0, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 0, 3.3, 0, 0, 0, 0, 0, 1.7;
    MatrixXd test_laplacian(5,5);
    //test_laplacian << ;
    MatrixXd test_sqrt_degree(5,5);
    test_sqrt_degree << 0.5590, 0, 0, 0, 0, 0, 0.8165, 0, 0, 0, 0, 0, 1.8257, 0, 0, 0, 0, 0, 0.5505, 0, 0, 0, 0, 0, 0.7670;
    VectorXd test_second_eigenvector; //= ;
    double test_second_eigenvalue; //= ;  //TODO: check these
    int test_diameter = 2;

    test_undirected_graph(G,
                          test_adjacency,
                          test_degree,
                          test_laplacian,
                          test_sqrt_degree,
                          test_second_eigenvector,
                          test_second_eigenvalue,
                          test_diameter);
}

bool test_undirected_graph(UndirectedGraph &G,
                           MatrixXd &Adjacency,
                           MatrixXd &Degree,
                           MatrixXd &Laplacian,
                           MatrixXd &SqrtDegree,
                           VectorXd second_eigenvector,
                           double second_eigenvalue,
                           int diameter){
    // Test Adjacency Matrix
    if(G.get_adjacency_matrix() != Adjacency){
        cout << "Adjacency matrix is not as expected." << endl;
        cout << "Expected:" << endl;
        cout << Adjacency << endl;
        cout << "Actual:" << endl;
        cout << G.get_adjacency_matrix() << endl;
        return false;
    }
    // Test Degree Matrix
    if(G.get_degree_matrix() != Degree){
        cout << "Degree matrix is not as expected." << endl;
        cout << "Expected:" << endl;
        cout << Degree << endl;
        cout << "Actual:" << endl;
        cout << G.get_degree_matrix() << endl;
        return false;
    }
    // Test Laplacian
//    if(G.get_laplacian_matrix() != Laplacian){
//        cout << "Laplacian matrix is not as expected." << endl;
//        cout << "Expected:" << endl;
//        cout << Laplacian << endl;
//        cout << "Actual:" << endl;
//        cout << G.get_laplacian_matrix() << endl;
//        return false;
//    }
    // Test SqrtDegree
    if(G.get_sqrt_degree_matrix() != SqrtDegree){
        cout << "Sqrt degree matrix is not as expected." << endl;
        cout << "Expected:" << endl;
        cout << SqrtDegree << endl;
        cout << "Actual:" << endl;
        cout << G.get_sqrt_degree_matrix() << endl;
        return false;
    }
    // Test 2nd EigenPair
//    auto second_eigenpair = G.get_second_eigenpair();
//    if(second_eigenpair.first != second_eigenvector){
//        cout << "Second EIGENVECTOR not as expected"<<endl;
//        cout << "Expected: "<< endl;
//        cout << second_eigenvector;
//        cout<< "Actual:"<<endl;
//        cout << second_eigenpair.first<<endl;
//        return false;
//    }
//    if(second_eigenpair.second != second_eigenvalue){
//        cout << "Second Eigenvalue not as expected" << endl;
//        cout << "Expected: " << second_eigenvalue << " Actual: " << second_eigenpair.second << endl;
//        return false;
//    }
    // Test Diameter
    if(G.get_estimated_diameter() != diameter){
        cout<< "DIAMETER not as expected" << endl;
        cout<< "Expected: " << diameter << " Actual: "<< G.get_estimated_diameter() << endl;
        return false;
    }
    // Test Edges Not Needed tested through graph conversion function
    // Test Nodes Not Needed tested through graph conversion function
    return true;
}

bool test_reading_hypergraph_from_database(HyperGraph H1,
                                           int number_of_nodes,
                                           int number_of_edges,
                                           int number_of_singleton_edges,
                                           map<string, vector<string>> predicate_argument_types,
                                           map<size_t, double> edge_weights,
                                           map<size_t, vector<size_t>> memberships,
                                           set<string> node_types,
                                           int number_of_predicates) {
    if (H1.number_of_nodes() != number_of_nodes){
        cout << "Checking that the hypergraph has an expected number of nodes";
        cout<< "Expected #nodes: " << number_of_nodes << "Actual " << H1.number_of_nodes() <<endl;
        return false;
    }
    if(H1.number_of_edges() != number_of_edges){
        cout << "Checking that the hypergraph has an expected number of edges";
        cout<< "Expected #edges: "<< number_of_edges << "Actual " << H1.number_of_edges() <<endl;
        return false;
    }
    if(H1.get_singleton_edges().size() != number_of_singleton_edges){
        cout << "Checking that the hypergraph has the expected number of singleton edges" << endl;
        cout<< "Expected #singleton_edges " << number_of_singleton_edges << "Actual " << H1.get_singleton_edges().size() <<endl;
        return false;
    }
    for(auto edge: get_keys(edge_weights)){
        if(H1.get_edge_weight(edge) != edge_weights[edge]){
            cout << "EDGE WEIGHT of EDGE"<< edge << "does not match expected weight"<<endl;
            cout << "Expected: " <<  edge_weights[edge] << " Actual: " << H1.get_edge_weight(edge) <<endl;
            return false;
        }
    }
    // Check number of predicates
    set<string> predicates;
    for(auto edge: get_keys(H1.get_edges())){
        predicates.insert(H1.get_predicate(edge).data());
    }
    if(predicates.size() != number_of_predicates){
        cout << "NUMBER of PREDICATES does not match expected number" <<endl;
        cout << "Expected: " << number_of_predicates << " Actual: "<< predicates.size() << endl;
        return false;
    }
    // Check memberships
    if(H1.get_memberships() != memberships){
        cout << "MEMBERSHIPS do not match expected memberships" << endl;
        return false;
    }
    if(H1.get_predicate_argument_types() != predicate_argument_types){
        cout<< "Predicate argument types map does not match the expected map"<<endl;
        cout<< "Expected: " << endl;
        for (const auto& pred : predicate_argument_types) {
            cout << pred.first << " : " << endl;
            for (const auto& argument_type : pred.second) {
                cout << argument_type << endl;
            }
        }
        cout<< "Actual: " << endl;
        for (const auto& pred : H1.get_predicate_argument_types()) {
            cout << pred.first << " : " << endl;
            for (const auto& argument_type : pred.second) {
                cout << argument_type << endl;
            }
        }
        return false;
    }
    if(H1.get_node_types() != node_types){
        cout<< "Node types do not match the expected node types."<<endl;
        cout<< "Expected: " << endl;
        for (auto node_type : node_types) {
            cout << node_type << endl;
        }
        cout<< "Actual: " << endl;
        for (auto node_type: H1.get_node_types()) {
            cout << node_type << endl;
        }
        return false;
    }

    return true;
}

bool test_graph_conversion(HyperGraph &H1) {
    /*
     * Test that a HyperGraph->Graph-Hypergraph conversion preserves the original hypergraph.
     * This should show that the conversion to and from graphs is correct.
     */
    UndirectedGraph G = UndirectedGraph(H1);

    if (G.number_of_nodes() != H1.number_of_nodes()){
        cout << "Checking number of nodes in the graph is the same as the hypergraph" << endl;
        cout << "Expected #nodes: " << H1.number_of_nodes() << "Actual " << G.number_of_nodes() << endl;
        return false;
    }

    HyperGraph H2(G, H1);

    // number of nodes
    if(H1.number_of_nodes() != H2.number_of_nodes()){
        cout<< "Number of NODES of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }
    // number of edges
    if(H1.number_of_edges() != H2.number_of_edges()){
        cout<< "Number of EDGES of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }
    // map<size_t, set<string>> singleton_edges; // node_id : set(predicate)
    if(H1.get_singleton_edges() != H2.get_singleton_edges()){
        cout<< "SINGLETON EDGES of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }
    // map<size_t, vector<size_t>> edges; // edge_id : list(node_id)

    if(H1.get_edges() != H2.get_edges()){
        cout<< "EDGES of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }
    // map<size_t, double> edge_weights; // edge_id : weight
    for(auto edge: get_keys(H1.get_edges())){
        if(H1.get_edge_weight(edge) != H2.get_edge_weight(edge)){
            cout<< "WEIGHT of EDGE"<< edge<<" of original hypergraph and reconstructed hypergraph are not the same."<<endl;
            return false;
        }
    }
    // map<size_t, string> predicates; // edge_id : predicate_name
    for(auto edge: get_keys(H1.get_edges())){
        if(H1.get_predicate(edge) != H2.get_predicate(edge)){
            cout<< "PREDICATE of EDGE" << edge << "original hypergraph and reconstructed hypergraph are not the same."<<endl;
            return false;
        }
    }
    //map<size_t, string> node_ids_names; // node_id : node_name
    if(H1.get_node_ids_names() != H2.get_node_ids_names()) {
        cout << "The MAPPING of Node IDS -> NAMES of original hypergraph and reconstructed hypergraph are not the same."
             << endl;
        return false;
    }
    //map<string, size_t> node_names_ids; // node_name : node_id
    if(H1.get_node_names_ids() != H2.get_node_names_ids()){
        cout<< "Node names ids of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }
    //map<size_t, string> nodes; // node_id : node_type
    if(H1.get_nodes() != H2.get_nodes()){
        cout<< "NODES of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }
    //map<size_t, vector<size_t>> memberships; // node_id : set(edge_id)
    if(H1.get_memberships() != H2.get_memberships()){
        cout<< "MEMBERSHIPS of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }
    //map<string, vector<string>> predicate_argument_types; // predicate_name : list(node_type)
    if(H1.get_predicate_argument_types() != H2.get_predicate_argument_types()){
        cout<< "PREDICATE ARGUMENT TYPES of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }
    //set<string> node_types; // set(node_type)
    if(H1.get_node_types() != H2.get_node_types()){
        cout<< "NODE TYPES of original hypergraph and reconstructed hypergraph are not the same."<<endl;
        return false;
    }

    return true;
}

