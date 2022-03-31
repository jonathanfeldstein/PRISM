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
        node_id_in_graph++;
    }

    this->adjacency_matrix = MatrixXd::Zero(graph_size, graph_size);
    this->degree_matrix = MatrixXd::Zero(graph_size, graph_size);
    for(vector<size_t> const &nodes : get_values(hypergraph.get_edges())){
        for(auto node_i = nodes.begin(); node_i < nodes.end(); node_i++){
            for(auto node_j = node_i+1; node_j < nodes.end(); node_j++){
                auto e = edge(*node_i, *node_j, graph);
                if(!e.second){
                    add_edge(*node_i, *node_j,1.0 ,graph);
                    this->adjacency_matrix(*node_i, *node_j) = 1;
                    this->adjacency_matrix(*node_j, *node_i) = 1;

                }else{
                    this->weightmap[e.first]++;
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
    this->laplacian_matrix = this->degree_matrix.pow(-0.5)*(this->degree_matrix-this->adjacency_matrix)*this->degree_matrix.pow(-0.5); //TODO Check correctness of power
}

UndirectedGraph::~UndirectedGraph() {
    cout<<"Destroy UndirectedGraph"<<endl;
}


int UndirectedGraph::estimate_diameter() {
    vector<int>dist_map(num_vertices(graph));
    typedef property_map<Graph, vertex_index_t>::type IdMap;
    iterator_property_map<vector<int>::iterator, IdMap, int, int&>distmap_vect(dist_map.begin(), get(vertex_index, G));
    Vertex source = 0;
    dijkstra_shortest_paths(graph, source, distance_map(distmap_vect)); // TODO check 0?
    Vertex farthest_vertex = max_element(dist_map.begin(), dist_map.end())-dist_map.begin();
    dijkstra_shortest_paths(graph, farthest_vertex, distance_map(distmap_vect); //TODO Check farthest_vertex?
    return *max_element(dist_map.begin(), dist_map.end());
}

int UndirectedGraph::number_of_nodes() {
    return num_vertices(graph);
}

int UndirectedGraph::number_of_edges() {
    return num_edges(graph);
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

int UndirectedGraph::get_estimated_diameter(){
    if(this->diameter != -1){
        return this->diameter;
    }else{
        return estimate_diameter();
    }
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

