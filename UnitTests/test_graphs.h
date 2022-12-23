#ifndef PRISM_TEST_GRAPHS_H
#define PRISM_TEST_GRAPHS_H

#include "HyperGraph.h"
#include "UndirectedGraph.h"
#include "test_utils.h"
#include "eigen_utils.h"

using namespace std;


pair<size_t, size_t>  TestHypergraph(const string& path_to_data);

pair<size_t, size_t> TestUndirectedGraph(const string& path_to_data);

TestReport test_reading_hypergraph_from_database(HyperGraph H1,
                                                 int number_of_nodes,
                                                 int number_of_edges,
                                                 int number_of_singleton_edges,
                                                 const map<string, vector<string>>& predicate_argument_types,
                                                 map<size_t, double> edge_weights,
                                                 const map<size_t, vector<size_t>>& memberships,
                                                 const set<string>& node_types,
                                                 int number_of_predicates);


TestReport test_graph_conversion(HyperGraph &H1);

TestReport test_undirected_graph(UndirectedGraph &G,
                                 MatrixXd &Adjacency,
                                 MatrixXd &Degree,
                                 MatrixXd &Laplacian,
                                 MatrixXd &SqrtDegree,
                                 const VectorXd& second_eigenvector,
                                 double second_eigenvalue,
                                 int diameter);

#endif //PRISM_TEST_GRAPHS_H
