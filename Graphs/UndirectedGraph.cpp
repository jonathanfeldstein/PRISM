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
    this->laplacian_matrix = this->degree_matrix.pow(-0.5)*(this->degree_matrix-this->adjacency_matrix)*this->degree_matrix.pow(-0.5);
}

UndirectedGraph::UndirectedGraph(UndirectedGraph &graph_template, set<size_t> subgraph_nodes){ //TODO check what happens if unconnected nodes fly arpund
    map<size_t, size_t> node_mapping; //Maps the position of the nodes in the old graph to the position of the nodes in the new graph
    this->adjacency_matrix = MatrixXd::Zero(subgraph_nodes.size(), subgraph_nodes.size());
    this->degree_matrix = MatrixXd::Zero(subgraph_nodes.size(), subgraph_nodes.size());
    for(auto node: subgraph_nodes){
        auto new_node = add_vertex(this->graph);
        this->graph[new_node].id = graph_template.graph[node].id;
        this->graph[new_node].name = graph_template.graph[node].name;
        node_mapping.insert({node, new_node});
    }
    for(auto node: subgraph_nodes){
        graph_traits<Graph>::out_edge_iterator edge_iterator, last_edge;
        for(tie(edge_iterator, last_edge) = out_edges(node, graph_template.graph); edge_iterator != last_edge; ++edge_iterator){
            Vertex source_node = source(*edge_iterator, graph_template.graph);
            Vertex target_node = target(*edge_iterator, graph_template.graph);
            if(subgraph_nodes.find(source_node)!=subgraph_nodes.end() && subgraph_nodes.find(target_node)!=subgraph_nodes.end()){
                auto e = edge(node_mapping[source(*edge_iterator, graph_template.graph)], node_mapping[target(*edge_iterator, graph_template.graph)], this->graph);
                if(!e.second){
                    add_edge(node_mapping[source(*edge_iterator, graph_template.graph)], node_mapping[target(*edge_iterator, graph_template.graph)], this->graph);
                    this->adjacency_matrix(node_mapping[source(*edge_iterator, graph_template.graph)], node_mapping[target(*edge_iterator, graph_template.graph)]) = graph_template.adjacency_matrix(source_node, target_node);
                    this->adjacency_matrix(node_mapping[target(*edge_iterator, graph_template.graph)], node_mapping[source(*edge_iterator, graph_template.graph)]) = graph_template.adjacency_matrix(target_node, source_node);
                }
            }
        }
    }
    size_t row_id {0};
    for(auto const &row:this->adjacency_matrix.rowwise()){
        this->degree_matrix(row_id, row_id) = row.sum();
        row_id++;
    }
    this->laplacian_matrix = this->degree_matrix.pow(-0.5)*(this->degree_matrix-this->adjacency_matrix)*this->degree_matrix.pow(-0.5);

}

UndirectedGraph::~UndirectedGraph() {
    cout<<"Destroy UndirectedGraph"<<endl;
}


int UndirectedGraph::estimate_diameter() {
    vector<int>dist_map(num_vertices(graph));
    typedef property_map<Graph, vertex_index_t>::type IdMap;
    iterator_property_map<vector<int>::iterator, IdMap, int, int&>distmap_vect(dist_map.begin(), get(vertex_index, graph));
    Vertex source = 0;
    dijkstra_shortest_paths(graph, source, distance_map(distmap_vect)); // TODO check 0?
    Vertex farthest_vertex = max_element(dist_map.begin(), dist_map.end())-dist_map.begin();
    dijkstra_shortest_paths(graph, farthest_vertex, distance_map(distmap_vect)); //TODO Check farthest_vertex?
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

vector<size_t> UndirectedGraph::sweep_set(Eigen::EigenSolver<MatrixXd>::EigenvalueType &second_EV, vector<size_t> degrees) {
    int best_cut_index{-1};
    double best_conductance{-1};
    size_t total_volume = accumulate(degrees.begin(), degrees.end(), 0);
    size_t set_volume{0};
    size_t set_size{0};
    size_t cut_weight{0};
    VectorXd normalize_second_EV = this->degree_matrix.pow(-0.5) * second_EV;
    // First sort the vertices based on their value in the second eigen vector
    vector<double> new_vector = to_vector(normalize_second_EV);
    vector<size_t> sorted_vertices = sort_indexes(new_vector);
    VectorXd weight_mask = VectorXd::Ones(this->number_of_nodes());
    size_t index{0};
    for(auto vertex: boost::adaptors::reverse(sorted_vertices)){
        set_volume +=degrees[vertex];
        set_size++;
        weight_mask[vertex]=-1;
        size_t additional_weight = this->adjacency_matrix.row(vertex).dot(weight_mask);
        cut_weight += additional_weight;
        double this_conductance = cut_weight/min(set_volume, total_volume-set_volume);
        if(best_conductance == -1 || this_conductance<best_conductance){
            best_conductance = this_conductance;
            best_cut_index = index;
        }
        index++;
    }
    return vector<size_t> (sorted_vertices.begin(), sorted_vertices.begin()+(best_cut_index+1)); //TODO check that slicing is correct
}

pair<UndirectedGraph, UndirectedGraph>
UndirectedGraph::cheeger_cut(Eigen::EigenSolver<MatrixXd>::EigenvalueType &second_EV) {
    vector<size_t> degrees(number_of_nodes());
    for(size_t node{0}; node<number_of_nodes(); node++){
        degrees.push_back(this->degree_matrix(node, node));
    }
    vector<size_t> vertices_indices_1 = sweep_set(second_EV, degrees);
    set<size_t> vertices1;
    for(size_t vertex{0}; vertex<number_of_nodes(); vertex++){ // TODO check whether we can do without it
        if(find(vertices_indices_1.begin(), vertices_indices_1.end(), vertex) != vertices_indices_1.end()){
            vertices1.insert(vertex);
        }
    }
    UndirectedGraph subgraph1(*this, vertices1);
    set<size_t> all_vertices;
    for (int i = 0; i < number_of_nodes(); ++i)
        all_vertices.insert(all_vertices.end(), i);
    set<size_t> vertices2;
    set_difference(all_vertices.begin(), all_vertices.end(), vertices1.begin(), vertices1.end(), vertices2.begin());
    UndirectedGraph subgraph2(*this, vertices2);
    return {subgraph1, subgraph2};
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





