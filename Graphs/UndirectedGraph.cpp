//
// Created by jonathan on 19/03/2022.
//

#include "UndirectedGraph.h"

using namespace std;
using namespace boost;
using namespace Eigen;
using Eigen::MatrixXd;

UndirectedGraph::UndirectedGraph() {
    cout<<"Creat a UndirectedGraph"<<endl;
};

UndirectedGraph::UndirectedGraph(HyperGraph &hypergraph) : graph(hypergraph.number_of_nodes()) {
    size_t graph_size = hypergraph.number_of_nodes();
    int node_id_in_graph{0};
    for(auto &node:hypergraph.get_node_ids_names()){
        graph[node_id_in_graph].id = node.first;
        graph[node_id_in_graph].name = node.second;
    }
    property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, graph);
    this->adjacency_matrix = MatrixXd::Zero(graph_size, graph_size);
    this->degree_matrix = MatrixXd::Zero(graph_size, graph_size);
    for(vector<size_t> const &nodes : get_values(hypergraph.get_edges())){
        for(auto node_i = nodes.begin(); node_i < nodes.end(); node_i++){
            for(auto node_j = node_i+1; node_j < nodes.end(); node_j++){
                auto e = edge(*node_i, *node_j, graph);
                if(!e.second){
                    auto new_edge = add_edge(*node_i, *node_j, graph);
                    weightmap[new_edge.first] = 1;
                    this->adjacency_matrix(*node_i, *node_j) = 1;
                    this->adjacency_matrix(*node_j, *node_i) = 1;

                }else{
                    weightmap[e.first]++;
                    this->adjacency_matrix(*node_i, *node_j)++;
                    this->adjacency_matrix(*node_j, *node_i)++;
                }
            }
        }
    }
    size_t row_id {0};
    for(auto const &row:this->adjacency_matrix.rowwise()){
        this->degree_matrix(row_id, row_id) = row.sum();
        row_id++;
    }
    this->laplacian_matrix = this->degree_matrix.pow(-0.5)*this->adjacency_matrix*this->degree_matrix.pow(-0.5); //TODO Check correctness of power
}

UndirectedGraph::~UndirectedGraph() {
    cout<<"Destroy UndirectedGraph"<<endl;
}


int UndirectedGraph::estimate_diameter() {
//    lemon::ListDigraph::ArcMap<double> costMap(this->graph);
//    lemon::ListDigraph::NodeMap<std::string> nodeMap(this->graph);
//    lemon::Dijkstra<lemon::ListDigraph, lemon::ListDigraph::ArcMap<double>> dijkstra(this->graph, costMap);
//    dijkstra.run(this->graph.nodeFromId(0));
//    map<int, double> cost = dijkstra.distMap();

    return 0;
}

int UndirectedGraph::number_of_nodes() {
    return 0;
}

int UndirectedGraph::number_of_edges() {
    return 0;
}

pair<Eigen::EigenSolver<MatrixXd>::EigenvalueType, complex<double>> UndirectedGraph::get_second_eigenpair() {
    Eigen::EigenSolver<MatrixXd> eigen_solver(this->laplacian_matrix);
    complex<double> second_eigen_value = eigen_solver.eigenvalues()[1];
    return {eigen_solver.eigenvectors().col(1), eigen_solver.eigenvalues()[1]};
}

map<size_t, string> UndirectedGraph::get_nodes() {
    map<size_t, string> node_ids_names;
    for(auto node : make_iterator_range(vertices(this->graph))){
        node_ids_names.insert({graph[node].id, graph[node].name });
    }

    return node_ids_names;
}

double get_estimated_diameter(){

    return 0;
}

void UndirectedGraph::print() {
    cout << "Adjacency Matrix\n";
    cout << this->adjacency_matrix;
    cout << endl;
    cout << "Degree Matrix\n";
    cout << this->degree_matrix;
    cout << endl;
    cout << "Laplacian Matrix\n";
    cout << this->laplacian_matrix;
    cout << endl;
}

