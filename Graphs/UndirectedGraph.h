//
// Created by jonathan on 19/03/2022.
//

#ifndef FASTER_UNDIRECTEDGRAPH_H
#define FASTER_UNDIRECTEDGRAPH_H
#include <iostream>
#include <list>
#include <tuple>
#include <vector>
#include <map>
#include <string>
#include <iterator>
#include <deque>
#include "HyperGraph.h"
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>
#include <boost/config.hpp>
#include <utility>
#include <math.h>

#include <boost/config.hpp>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include "../Utils/vector_utils.h"
#include "../Utils/profiling_utils.h"

#include <Spectra/SymEigsShiftSolver.h>
#include <Spectra/SymEigsSolver.h>
using namespace std;
using namespace boost;
using Eigen::MatrixXd;

class HyperGraph;
struct VertexData{
    string name;
    size_t id;
};
typedef adjacency_list<vecS, vecS, undirectedS, VertexData, property<edge_weight_t, double>> Graph;

//vertex and edge descriptor
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

class UndirectedGraph {
private:
    Graph graph;
    property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, graph);
    MatrixXd adjacency_matrix;
    MatrixXd degree_matrix;
    MatrixXd laplacian_matrix;
    MatrixXd sqrt_degree;
    bool diameter_computed = false;
    int diameter{0};


public:
    UndirectedGraph();
    UndirectedGraph(HyperGraph &hypergraph);
    UndirectedGraph(UndirectedGraph &graph_template, set<size_t> subgraph_nodes);
    ~UndirectedGraph();
    MatrixXd get_adjacency_matrix();
    MatrixXd get_laplacian_matrix();
    MatrixXd get_degree_matrix();
    MatrixXd get_sqrt_degree_matrix();
    int number_of_nodes();
    int number_of_edges();
    int estimate_diameter();
    map<size_t, string> get_nodes();
    pair<VectorXd, double> get_second_eigenpair();
    int get_estimated_diameter();
    set<size_t> sweep_set(VectorXd &second_EV, vector<size_t> degrees);
    pair<UndirectedGraph, UndirectedGraph> cheeger_cut(VectorXd &second_EV);
    void print();
};
#endif //FASTER_UNDIRECTEDGRAPH_H
