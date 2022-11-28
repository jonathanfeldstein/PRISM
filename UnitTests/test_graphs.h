//
// Created by dominic on 01/05/22.
//

#ifndef PRISM_TEST_GRAPHS_H
#define PRISM_TEST_GRAPHS_H

#include "HyperGraph.h"
#include "UndirectedGraph.h"

using namespace std;


bool TestHypergraph(string path_to_data);

bool TestUndirectedGraph(string path_to_data);

bool test_reading_hypergraph_from_database(HyperGraph H1,
                                           int number_of_nodes,
                                           int number_of_edges,
                                           int number_of_singleton_edges,
                                           map<string, vector<string>> predicate_argument_types,
                                           map<size_t, double> edge_weights,
                                           map<size_t, vector<size_t>> memberships,
                                           set<string> node_types,
                                           int number_of_predicates);


bool test_graph_conversion(HyperGraph &H1);

bool test_undirected_graph(UndirectedGraph &G,
                           MatrixXd &Adjacency,
                           MatrixXd &Degree,
                           MatrixXd &Laplacian,
                           MatrixXd &SqrtDegree,
                           VectorXd second_eigenvector,
                           double second_eigenvalue,
                           int diameter);

#endif //PRISM_TEST_GRAPHS_H
