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

using namespace std;
using namespace boost;
using Eigen::MatrixXd;

class HyperGraph;
struct VertexData{
    string name;
    size_t id;
};
typedef adjacency_list<vecS, vecS, undirectedS, VertexData, property<edge_weight_t, double>> Graph;


class UndirectedGraph {
private:
    Graph graph;
    MatrixXd adjacency_matrix;
    MatrixXd degree_matrix;
    MatrixXd laplacian_matrix;


public:
    UndirectedGraph();
    UndirectedGraph(HyperGraph &hypergraph);
    ~UndirectedGraph();
    int number_of_nodes();
    int number_of_edges();
    int estimate_diameter();
    map<size_t, string> get_nodes();
    pair<Eigen::EigenSolver<MatrixXd>::EigenvalueType, complex<double>> get_second_eigenpair();
    double get_estimated_diameter();

};
#endif //FASTER_UNDIRECTEDGRAPH_H
