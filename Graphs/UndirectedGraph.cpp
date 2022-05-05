//
// Created by jonathan on 19/03/2022.
//

#include "UndirectedGraph.h"

UndirectedGraph::UndirectedGraph() = default;

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
    for(auto &hyperedge : hypergraph.get_edges()){
        for(auto node_i = hyperedge.second.begin(); node_i < hyperedge.second.end(); node_i++){
            for(auto node_j = node_i+1; node_j < hyperedge.second.end(); node_j++){
                auto e = edge(*node_i, *node_j, graph);
                if(!e.second){
                    add_edge(*node_i, *node_j,1.0 ,graph);
                    this->adjacency_matrix(*node_i, *node_j) = hypergraph.get_edge_weight(hyperedge.first);
                    this->adjacency_matrix(*node_j, *node_i) = hypergraph.get_edge_weight(hyperedge.first);

                }else{
                    this->adjacency_matrix(*node_i, *node_j) += hypergraph.get_edge_weight(hyperedge.first);
                    this->adjacency_matrix(*node_j, *node_i) += hypergraph.get_edge_weight(hyperedge.first);
                }
            }
        }
    }
    size_t row_id {0};
    for(auto const &row:this->adjacency_matrix.rowwise()){
        this->degree_matrix(row_id, row_id) = row.sum();
        row_id++;
    }
    this->sqrt_degree = MatrixXd::Zero(row_id, row_id);
    for(int i{0}; i<sqrt_degree.rows(); i++){
        sqrt_degree(i, i) = pow(this->degree_matrix(i, i), -0.5);
    }
    this->laplacian_matrix = this->sqrt_degree*(this->degree_matrix-this->adjacency_matrix)*this->sqrt_degree;
}

UndirectedGraph::UndirectedGraph(UndirectedGraph &graph_template, set<NodeId> subgraph_nodes){
    map<NodeId, NodeId> node_mapping; //Maps the position of the nodes in the old graph to the position of the nodes in the new graph
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
                    add_edge(node_mapping[source(*edge_iterator, graph_template.graph)], node_mapping[target(*edge_iterator, graph_template.graph)], 1.0, this->graph);
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
    this->sqrt_degree = MatrixXd::Zero(row_id, row_id);
    for(int i{0}; i<sqrt_degree.rows(); i++){
        sqrt_degree(i, i) = pow(this->degree_matrix(i, i), -0.5);
    }
    this->laplacian_matrix = this->sqrt_degree*(this->degree_matrix-this->adjacency_matrix)*this->sqrt_degree;

}

UndirectedGraph::~UndirectedGraph() = default;

MatrixXd UndirectedGraph::get_adjacency_matrix() {
    return this->adjacency_matrix;
}

MatrixXd UndirectedGraph::get_laplacian_matrix() {
    return this->laplacian_matrix;
}

MatrixXd UndirectedGraph::get_degree_matrix() {
    return this->degree_matrix;
}

MatrixXd UndirectedGraph::get_sqrt_degree_matrix() {
    return this->sqrt_degree;
}

size_t UndirectedGraph::estimate_diameter() {
    vector<int>dist_map(num_vertices(graph));
    typedef property_map<Graph, vertex_index_t>::type IdMap;
    iterator_property_map<vector<int>::iterator, IdMap, int, int&>distmap_vect(dist_map.begin(), get(vertex_index, graph));
    Vertex source = 0;
    dijkstra_shortest_paths(graph, source, distance_map(distmap_vect));
    Vertex farthest_vertex = max_element(dist_map.begin(), dist_map.end())-dist_map.begin();
    dijkstra_shortest_paths(graph, farthest_vertex, distance_map(distmap_vect));
    this->diameter_computed = true;
    return *max_element(dist_map.begin(), dist_map.end());
}

int UndirectedGraph::number_of_nodes() {
    return num_vertices(graph);
}

int UndirectedGraph::number_of_edges() {
    return num_edges(graph);
}

pair<VectorXd, double> UndirectedGraph::get_second_eigenpair() {
    //Remark, as we only operate on symmetric matrices all EVs are real
    Timer ES("ES instantiation");
    Spectra::DenseSymShiftSolve<double> op(this->laplacian_matrix);
    Spectra::SymEigsShiftSolver<Spectra::DenseSymShiftSolve<double>> eigen_solver(op, 2,3, 0.001);
    eigen_solver.init();
    eigen_solver.compute(Spectra::SortRule::LargestMagn);

    VectorXd evalues = eigen_solver.eigenvalues();
    MatrixXd evectors = eigen_solver.eigenvectors();
    double second_eigen_value = evalues(0);
    VectorXd second_eigen_vector = evectors.col(0);
    ES.Stop();
    return {second_eigen_vector, second_eigen_value};
}

map<NodeId, NodeName> UndirectedGraph::get_nodes() {
    map<size_t, NodeName> node_ids_names;
    for(auto node : make_iterator_range(vertices(this->graph))){
        node_ids_names.insert({graph[node].id, graph[node].name });
    }

    return node_ids_names;
}

size_t UndirectedGraph::get_estimated_diameter(){
    if(this->diameter_computed){
        return this->diameter;
    }else{
        return estimate_diameter();
    }
}

set<NodeId> UndirectedGraph::sweep_set(VectorXd &second_EV, vector<size_t> degrees) {
    int best_cut_index{-1};
    double best_conductance{-1};
    double total_volume = accumulate(degrees.begin(), degrees.end(), 0);
    double set_volume{0};
    double set_size{0};
    double cut_weight{0};
    VectorXd normalize_second_EV = this->sqrt_degree * second_EV;
    // First sort the vertices based on their value in the second eigen vector
    vector<double> new_vector = to_vector(normalize_second_EV);
    vector<size_t> sorted_vertices = sort_indexes(new_vector, true);
    VectorXd weight_mask = VectorXd::Ones(this->number_of_nodes());
    size_t index{0};
    for(auto i =0; i <sorted_vertices.size()-1; i++){
        set_volume +=degrees[sorted_vertices[i]];
        set_size++;
        weight_mask[sorted_vertices[i]]=-1;
        double additional_weight = this->adjacency_matrix.row(sorted_vertices[i]).dot(weight_mask);
        cut_weight += additional_weight;
        double this_conductance = cut_weight/min(set_volume, total_volume-set_volume);
        if(best_conductance == -1 || this_conductance<best_conductance){
            best_conductance = this_conductance;
            best_cut_index = index;
        }
        index++;
    }
    return set<size_t> (sorted_vertices.begin(), sorted_vertices.begin()+(best_cut_index+1)); //TODO check that slicing is correct
}

pair<UndirectedGraph, UndirectedGraph>
UndirectedGraph::cheeger_cut(VectorXd &second_EV) {
    vector<size_t> degrees{};
    for(NodeId node{0}; node<number_of_nodes(); node++){
        degrees.push_back(this->degree_matrix(node, node));
    }
    set<NodeId> vertices1 = sweep_set(second_EV, degrees);
    UndirectedGraph subgraph1(*this, vertices1);
    set<NodeId> all_vertices;
    for (int i = 0; i < number_of_nodes(); ++i)
        all_vertices.insert(all_vertices.end(), i);
    set<NodeId> vertices2;
    set_difference(all_vertices.begin(), all_vertices.end(), vertices1.begin(), vertices1.end(), inserter(vertices2, vertices2.end()));
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




