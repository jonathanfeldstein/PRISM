//
// Created by jonathan on 19/03/2022.
//
#include "map_utils.h"

#include "HyperGraph.h"

using namespace std;


HyperGraph::HyperGraph() = default;

HyperGraph::HyperGraph(string const& db_file_path, string const& info_file_path) {
    set_predicate_argument_types_from_file(info_file_path);
    fstream db_file;
    db_file.open(db_file_path, ios::in);
    if(db_file.is_open()){
        string line;
        size_t edge_id{0};
        while(getline(db_file, line)){
            pair<string, vector<string>> predicate_node_names= parse_line(line);
            vector<size_t> node_ids_in_edge;
            for(auto &argument: predicate_node_names.second){
                if(!this->node_names_ids.count(argument)){
                    size_t node_id = node_names_ids.size();
                    this->node_names_ids[argument] = node_id;
                    this->node_ids_names[node_id] = argument;
                }
                node_ids_in_edge.push_back(this->node_names_ids[argument]);
            }
            if(node_ids_in_edge.size()>1){
                add_edge(edge_id, predicate_node_names.first, node_ids_in_edge);
                edge_id++;
            }else{
                add_edge(predicate_node_names.first, node_ids_in_edge.front());
            }

        }
        db_file.close();
    }// TODO throw exception otherwise
    for(auto const& node_name : get_keys(nodes)){
        is_source_node[node_name] = true;
    }
    // TODO assert is_connected
}


HyperGraph::HyperGraph(UndirectedGraph &graph, HyperGraph &hypergraph_template) {

    this->predicate_argument_types = hypergraph_template.predicate_argument_types;
    this->estimated_graph_diameter = graph.get_estimated_diameter();
    for(auto &node : graph.get_nodes()){
        size_t node_id = node.first;
        // add non-singleton edges to the hypergraph
        vector<size_t> hyperedges_of_node = hypergraph_template.get_memberships(node_id);
        for(auto edge: hyperedges_of_node){
            string predicate = hypergraph_template.get_predicate(edge);
            vector<size_t> nodes_of_hyperedge = hypergraph_template.get_nodes_of_edge(edge);

            // only add a hyperedge if a strict majority of vertices in the edge are part of the cluster
            set<size_t> graph_nodes(get_keys(graph.get_nodes()));
            set<size_t> hypergraph_nodes(nodes_of_hyperedge.begin(), nodes_of_hyperedge.end());
            set<size_t> overlapping_nodes;
            set_intersection(graph_nodes.begin(), graph_nodes.end(), hypergraph_nodes.begin(), hypergraph_nodes.end(), inserter(overlapping_nodes, overlapping_nodes.begin()));
            size_t number_of_edge_nodes_in_graph = overlapping_nodes.size();
            if(number_of_edge_nodes_in_graph > (hypergraph_nodes.size() / 2)){
                for(auto node_of_hyperedge:nodes_of_hyperedge){
                    if(!this->node_ids_names.count(node_of_hyperedge)){
                        string name = hypergraph_template.node_ids_names[node_of_hyperedge];
                        this->node_ids_names[node_of_hyperedge] = name;
                        this->node_names_ids[name] = node_of_hyperedge;
                    }
                }
                if(!this->edges.count(edge)){
                    this->add_edge(edge, predicate, nodes_of_hyperedge);
                }
            }
            vector<string> argument_types = hypergraph_template.get_predicate_argument_types(predicate);
            set<string> new_node_types(argument_types.begin(), argument_types.end());
            this->node_types.merge(new_node_types);
        }
        this->is_source_node[node_id] = true;
        // add singleton edges to the hypergraph
        map<size_t, set<string>> node_singleton_edges = hypergraph_template.get_singleton_edges();
        for(auto &predicate:node_singleton_edges[node_id]){
            this->add_edge(predicate, node_id);
        }
    }
}

HyperGraph::~HyperGraph() = default;

void HyperGraph::set_predicate_argument_types_from_file(string const& info_file_path) {
    fstream info_file;
    info_file.open(info_file_path, ios::in);
    if(info_file.is_open()){
        string line; // TODO Test if empty lines are skipped and make comments skipped
        while(getline(info_file, line)){
            pair<string, vector<string>> predicate_arguments= parse_line(line);
            predicate_argument_types[predicate_arguments.first] = predicate_arguments.second;
            set<string> arguments_set;
            copy(predicate_arguments.second.begin(),
                    predicate_arguments.second.end(),
                    inserter(arguments_set, arguments_set.begin())); //Cast list into set

            node_types.merge(arguments_set); // Updating node_types
        }
        info_file.close();
    }// TODO throw exception otherwise
}

pair<string, vector<string>> HyperGraph::parse_line(string line) {
    line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end()); //Strip White Space
    size_t pos = 0;
    vector<string> arguments;
    string predicate;
    while ((pos = line.find_first_of(",()")) != std::string::npos) { //Split the line by ',' or '(' or ')'
        if (predicate.empty()){
            predicate = line.substr(0, pos);
        }else{
            arguments.push_back(line.substr(0, pos));
        }
        line.erase(0, pos + 1);
    }
    return {predicate, arguments};
}

bool HyperGraph::is_connected() { // TODO Does not work
    set<size_t> singleton_edges_keys = get_keys(singleton_edges);
    set<size_t> nodes_keys = get_keys(nodes);
    vector<size_t> intersection;
    set_intersection(singleton_edges_keys.begin(), singleton_edges_keys.end(), nodes_keys.begin(), nodes_keys.end(), intersection.begin());

    // The hypergraph is not connected if there exists a node which only belongs to singleton edges
    bool is_connected = intersection.size() >= singleton_edges_keys.size();
    return is_connected;
}

bool HyperGraph::check_is_source_node(int node_id) {
    return this->is_source_node[node_id];
}

void HyperGraph::add_edge(size_t edge_id, string const& predicate, vector<size_t> node_ids) { //TODO remark split up singleton edges now no safeguard about inserting here a singleton edge
        this->edges[edge_id] = node_ids;
        this->predicates[edge_id] = predicate;
        for(auto const& node_id: node_ids){
            this->memberships[node_id].push_back(edge_id);
        }
        for(size_t i{0}; i < node_ids.size(); i++){
            string const& node_type = this->predicate_argument_types[predicate][i];
            this->nodes[node_ids[i]] = node_type;
        }
}

void HyperGraph::add_edge(const string &predicate, size_t node_id) {
    this->singleton_edges[node_id].insert(predicate);
}

map<size_t, set<string>> HyperGraph::get_singleton_edges() {
    return this->singleton_edges;
}

map<size_t, vector<size_t>> HyperGraph::get_edges() {
    return this->edges;
}

set<size_t> HyperGraph::get_node_ids() {
    set<size_t> all_nodes = get_keys(nodes);
    all_nodes.merge(get_keys(singleton_edges)); //TODO check that singleton edges does not get destroyed
    return all_nodes;
}

map<size_t, string> HyperGraph::get_nodes() {
    return this->nodes;
}

vector<size_t> HyperGraph::get_nodes_of_edge(size_t edge_id) {
    return this->edges[edge_id];
}

map<size_t, string> HyperGraph::get_predicates() {
    return this->predicates;
}

string HyperGraph::get_predicate(size_t edge_id) {
    return this->predicates[edge_id];
}

set<string> HyperGraph::get_node_types() {
    return this->node_types;
}

map<size_t, vector<size_t>> HyperGraph::get_memberships() {
    return this->memberships;
}
vector<size_t> HyperGraph::get_memberships(size_t node_id){
    return this->memberships[node_id];
}

size_t HyperGraph::number_of_nodes() {
    set<size_t> all_nodes = this->get_node_ids();
    return all_nodes.size();
}

size_t HyperGraph::number_of_edges() {
    size_t number_of_singleton_edges {0};
    for(set<string> const& predicate_set:get_values(singleton_edges)){
        number_of_singleton_edges += predicate_set.size();
    }
    return number_of_singleton_edges + edges.size();
}

int HyperGraph::number_of_predicates() {
    vector<string> predicate_values = get_values(predicates);
    set<std::string> predicate_values_as_set(predicate_values.begin(), predicate_values.end());
    return predicate_values_as_set.size();
}

int HyperGraph::get_estimated_graph_diameter() {
    return this->estimated_graph_diameter;
}

pair<size_t, size_t> HyperGraph::get_random_edge_and_neighbor_of_node(size_t const& node) {
    vector<size_t> potential_edges = this->memberships[node]; //TODO transform to vector
    size_t edge_id = uniform_random(potential_edges.size()-1);
    size_t chosen_edge = potential_edges[edge_id];
    vector<size_t> nodes_of_edge = this->edges[chosen_edge];
    // Find the node in the vector, as we don't want to select the same node in the next step
    vector<size_t>::iterator position = find(nodes_of_edge.begin(), nodes_of_edge.end(), node); //find node in vector
    nodes_of_edge.erase(position); //remove the node by index
    size_t neighbor_id = uniform_random(nodes_of_edge.size()-1);
    size_t neighbor = nodes_of_edge[neighbor_id];
    return {chosen_edge, neighbor};
}

map<string, vector<string>> HyperGraph::get_predicate_argument_types() {
    return this->predicate_argument_types;
}

vector<string> HyperGraph::get_predicate_argument_types(string predicate) {
    return this->predicate_argument_types[predicate];
}

map<size_t, string> HyperGraph::get_node_ids_names() {
    return this->node_ids_names;
}


void HyperGraph::print() {
    cout << "Node Types\n";
    for(const auto& node_type: this->node_types){
        cout << node_type << " ";
    }
    cout << endl;
    cout << endl;
    cout << "Nodes\n";
    for(const auto& node: this->nodes){
        cout << node.first << " " << node.second << "\n";
    }
    cout << endl;
    cout << "Node name ids\n";
    for(const auto& node: this->node_names_ids){
        cout << node.first << " " << node.second << "\n";
    }
    cout << endl;
    cout << "Singleton Edges (node id | predicates) \n";
    for(const auto& singleton_edge: this->singleton_edges){
        cout << singleton_edge.first << " | ";
        for (const auto& predicate : singleton_edge.second) {
            cout << predicate << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Edges (edge id | node ids)\n";
    for(const auto& edge: this->edges){
        cout << edge.first << " | ";
        for (auto node : edge.second) {
            cout << node << " ";
        }
        cout << "\n";
    }
    cout << endl;
    cout << "Predicates (edge id | predicate)\n";
    for(const auto& predicate: this->predicates){
        cout << predicate.first << " | " << predicate.second << "\n";
    }
    cout << endl;
    cout << "Predicate Argument Types (predicate | argument types)\n";
    for(const auto& predicate: this->predicate_argument_types){
        cout << predicate.first << " | ";
        for(const auto& argument_type: predicate.second) {
            cout << argument_type << " ";
        }
        cout << "\n";
    }
    cout << endl;
    cout<<"Diameter"<<endl;
    cout<<this->estimated_graph_diameter<<endl;
}
