#include "map_utils.h"

#include "HyperGraph.h"

using namespace std;


HyperGraph::HyperGraph() = default;

HyperGraph::HyperGraph(string const& db_file_path, string const& info_file_path, bool safe) {
    if(!file_exists(db_file_path)){
        throw FileNotFoundException(db_file_path);
    } else if(!file_exists(info_file_path)){
        throw FileNotFoundException(info_file_path);
    }else{
        set_predicate_argument_types_from_file(info_file_path, safe);
        fstream db_file;
        db_file.open(db_file_path, ios::in);
        if(db_file.is_open()){
            string line;
            EdgeId edge_id{0};
            while(getline(db_file, line)){
                if((line[0] == '/' && line[1] == '/') || line.empty()){ // Escaping commented lines and empty lines
                    continue;
                }
                GroundRelation relation = parse_line_db(line, safe);
                vector<NodeId> node_ids_in_edge;
                for(auto &argument: relation.arguments){
                    if(!this->node_names_ids.count(argument)){
                        NodeId node_id = this->node_names_ids.size();
                        this->node_names_ids[argument] = node_id;
                        this->node_ids_names[node_id] = argument;
                    }
                    node_ids_in_edge.push_back(this->node_names_ids[argument]);
                }
                add_edge(edge_id, relation.predicate, node_ids_in_edge, relation.weight);
                edge_id++;
            }
            db_file.close();
        }else{
            throw FileNotOpenedException(db_file_path);
        }
        if(this->number_of_nodes() < 3){
            throw HyperGraphSizeException();
        }
        for(auto const& node_name : get_keys(nodes)){
            is_source_node[node_name] = true;
        }
    }
}


HyperGraph::HyperGraph(UndirectedGraph &graph, HyperGraph &hypergraph_template) {

    this->predicate_argument_types = hypergraph_template.predicate_argument_types;
    this->estimated_graph_diameter = graph.get_estimated_diameter();
    for(auto &node : graph.get_nodes()){
        NodeId node_id = node.first;
        vector<EdgeId> hyperedges_of_node = hypergraph_template.get_memberships(node_id);
        for(auto edge: hyperedges_of_node){
            Predicate predicate = hypergraph_template.get_predicate(edge).data();
            vector<NodeId> nodes_of_hyperedge = hypergraph_template.get_nodes_of_edge(edge);

            // only add a hyperedge if a strict majority of vertices in the edge are part of the cluster
            set<NodeId> graph_nodes(get_keys(graph.get_nodes()));
            set<NodeId> hypergraph_nodes(nodes_of_hyperedge.begin(), nodes_of_hyperedge.end());
            set<NodeId> overlapping_nodes;
            set_intersection(graph_nodes.begin(), graph_nodes.end(),
                             hypergraph_nodes.begin(), hypergraph_nodes.end(),
                             inserter(overlapping_nodes, overlapping_nodes.begin()));
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
                    this->add_edge(edge, predicate, nodes_of_hyperedge, hypergraph_template.edge_weights[edge]);
                }
            }
            vector<NodeType> argument_types = hypergraph_template.get_predicate_argument_types(predicate);
            set<NodeType> new_node_types(argument_types.begin(), argument_types.end());
            this->node_types.merge(new_node_types);
        }
        this->is_source_node[node_id] = true;
    }
    if(!this->is_connected()){
        throw HyperGraphConnectedException();
    }
}

HyperGraph::~HyperGraph() = default;

void HyperGraph::set_predicate_argument_types_from_file(string const& info_file_path, bool safe) {
    fstream info_file;
    info_file.open(info_file_path, ios::in);
    if(info_file.is_open()){
        string line;
        while(getline(info_file, line)){
            if((line[0] == '/' && line[1] == '/') || line.empty()){ // Escaping commented lines and empty lines
                continue;
            }
            Relation relation= parse_line_info(line, safe);
            predicate_argument_types[relation.predicate] = relation.arguments;
            set<NodeType> arguments_set(relation.arguments.begin(),
                                        relation.arguments.end());
            node_types.merge(arguments_set); // Updating node_types
        }
        info_file.close();
    }else{
        throw FileNotOpenedException(info_file_path);
    }
}


bool HyperGraph::is_connected() {
    bool is_connected = false;
    // DFS
    NodeId source_node = *(get_keys(nodes).begin());
    set<NodeId> connected_nodes = dfs(source_node);

    if(connected_nodes == get_keys(nodes)){
        is_connected = true;
    }
    return is_connected;
}

bool HyperGraph::check_is_source_node(NodeId node_id) {
    return this->is_source_node[node_id];
}

void HyperGraph::add_edge(EdgeId edge_id, Predicate const& predicate, vector<NodeId> node_ids, double weight) {
        this->edges[edge_id] = node_ids;
        this->edge_weights[edge_id] = weight;
        this->predicates[edge_id] = predicate;
        for(auto const& node_id: node_ids){
            this->memberships[node_id].push_back(edge_id);
        }
        for(size_t i{0}; i < node_ids.size(); i++){
            string const& node_type = this->predicate_argument_types[predicate][i];
            this->nodes[node_ids[i]] = node_type;
        }
}

map<EdgeId, vector<NodeId>>& HyperGraph::get_edges() {
    return this->edges;
}
vector<NodeId> HyperGraph::get_edge(EdgeId edge_id) {
    return this->edges[edge_id];
}

set<NodeId> HyperGraph::get_node_ids() {
    return get_keys(nodes);
}

map<NodeId, NodeType> HyperGraph::get_nodes() {
    return this->nodes;
}

vector<NodeId> HyperGraph::get_nodes_of_edge(EdgeId edge_id) {
    return this->edges[edge_id];
}

string_view HyperGraph::get_predicate(EdgeId edge_id) {
    return this->predicates[edge_id];
}

set<NodeType> HyperGraph::get_node_types() {
    return this->node_types;
}

map<NodeId, vector<EdgeId>> HyperGraph::get_memberships() {
    return this->memberships;
}
vector<EdgeId> HyperGraph::get_memberships(NodeId node_id){
    return this->memberships[node_id];
}

size_t HyperGraph::number_of_nodes() {
    set<NodeId> all_nodes = this->get_node_ids();
    return all_nodes.size();
}

size_t HyperGraph::number_of_edges() {
    return edges.size();
}

int HyperGraph::number_of_predicates() {
    vector<Predicate> predicate_values = get_values(predicates);
    set<Predicate> predicate_values_as_set(predicate_values.begin(), predicate_values.end());
    return predicate_values_as_set.size();
}

size_t HyperGraph::get_estimated_graph_diameter() const {
    return this->estimated_graph_diameter;
}

pair<EdgeId, NodeId> HyperGraph::get_random_edge_and_neighbor_of_node(size_t const& node) {
    vector<EdgeId> potential_edges = this->memberships[node];
    vector<double> potential_edges_weights;
    for(auto edge: potential_edges){
        potential_edges_weights.emplace_back(this->get_edge_weight(edge));
    }
    size_t edge_index = weighted_discrete_distribution(potential_edges_weights);
    EdgeId edge_id = potential_edges[edge_index];
    vector<NodeId> nodes_of_edge = this->edges[edge_id];
    NodeId node_id;
    if(nodes_of_edge.size() == 1){ // If the edge is a singleton, choose same node
        node_id = node;
    }else{
        // Find the node in the vector, as we don't want to select the same node in the next step
        auto position = find(nodes_of_edge.begin(), nodes_of_edge.end(), node); //find origin node in vector
        nodes_of_edge.erase(position); //remove the node by index
        size_t node_index = uniform_random_int(nodes_of_edge.size());
        node_id = nodes_of_edge[node_index];
    }
    return {edge_id, node_id};
}

map<Predicate, vector<NodeType>> HyperGraph::get_predicate_argument_types() {
    return this->predicate_argument_types;
}

vector<NodeType> HyperGraph::get_predicate_argument_types(Predicate &predicate) {
    return this->predicate_argument_types[predicate];
}

map<NodeId, NodeName> HyperGraph::get_node_ids_names() {
    return this->node_ids_names;
}

map<NodeName, NodeId> HyperGraph::get_node_names_ids() {
    return this->node_names_ids;
}

double HyperGraph::get_edge_weight(EdgeId edge_id) {
    return this->edge_weights[edge_id];
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

void HyperGraph::compute_diameter() {
    UndirectedGraph graph(*this);
    size_t diameter = graph.get_estimated_diameter();
    this->estimated_graph_diameter = diameter;
}

vector<set<NodeId>> HyperGraph::find_unconnected_components() {
    set<NodeId> unassigned_nodes = get_keys(nodes);
    vector<set<NodeId>> unconnected_components;
    // DFS
    while(!unassigned_nodes.empty()){
        NodeId source_node = *(unassigned_nodes.begin());
        set<NodeId> connected_nodes = dfs(source_node);
        unconnected_components.emplace_back(connected_nodes);
        set<NodeId> remaining_nodes(unassigned_nodes.begin(), unassigned_nodes.end());
        unassigned_nodes.clear();
        set_difference(remaining_nodes.begin(), remaining_nodes.end(),
                       connected_nodes.begin(), connected_nodes.end(),
                       std::inserter(unassigned_nodes, unassigned_nodes.end()));
    }
    return unconnected_components;
}

set<NodeId> HyperGraph::dfs(NodeId source_node) {
    set<NodeId> current_nodes;
    set<NodeId> connected_nodes;
    current_nodes.insert(source_node);
    connected_nodes.insert(source_node);
    while(!current_nodes.empty()){
        set<NodeId> next_nodes;
        for(auto node: current_nodes){
            vector<EdgeId> edge_ids = get_memberships(node);
            for(auto edge:edge_ids){
                for(auto new_node: get_edge(edge)){
                    if(!has(connected_nodes, new_node)){
                        connected_nodes.insert(new_node);
                        next_nodes.insert(new_node);
                    }
                }
            }
        }
        current_nodes.clear();
        current_nodes = next_nodes;
    }
    return connected_nodes;
}

HyperGraph::HyperGraph(set<NodeId> nodes_subset, HyperGraph &hypergraph_template) {

    this->predicate_argument_types = hypergraph_template.predicate_argument_types;
    for(auto node_id : nodes_subset){
//        NodeId node_id = node.first;
        vector<EdgeId> hyperedges_of_node = hypergraph_template.get_memberships(node_id);
        for(auto edge: hyperedges_of_node){
            Predicate predicate = hypergraph_template.get_predicate(edge).data();
            vector<NodeId> nodes_of_hyperedge = hypergraph_template.get_nodes_of_edge(edge);

            // only add a hyperedge if a strict majority of vertices in the edge are part of the cluster
//            set<NodeId> graph_nodes(get_keys(graph.get_nodes()));
//            set<NodeId> hypergraph_nodes(nodes_of_hyperedge.begin(), nodes_of_hyperedge.end());
//            set<NodeId> overlapping_nodes;
//            set_intersection(graph_nodes.begin(), graph_nodes.end(),
//                             hypergraph_nodes.begin(), hypergraph_nodes.end(),
//                             inserter(overlapping_nodes, overlapping_nodes.begin()));
//            size_t number_of_edge_nodes_in_graph = overlapping_nodes.size();
//            if(number_of_edge_nodes_in_graph > (hypergraph_nodes.size() / 2)){
                for(auto node_of_hyperedge:nodes_of_hyperedge){
                    if(!this->node_ids_names.count(node_of_hyperedge)){
                        string name = hypergraph_template.node_ids_names[node_of_hyperedge];
                        this->node_ids_names[node_of_hyperedge] = name;
                        this->node_names_ids[name] = node_of_hyperedge;
                    }
                }
                if(!this->edges.count(edge)){
                    this->add_edge(edge, predicate, nodes_of_hyperedge, hypergraph_template.edge_weights[edge]);
                }
//            }
            vector<NodeType> argument_types = hypergraph_template.get_predicate_argument_types(predicate);
            set<NodeType> new_node_types(argument_types.begin(), argument_types.end());
            this->node_types.merge(new_node_types);
        }
        this->is_source_node[node_id] = true;
    }
    if(!this->is_connected()){
        throw HyperGraphConnectedException();
    }
//    if(this->number_of_nodes() < 3) {
//        throw HyperGraphSizeException();
//    }
}

vector<HyperGraph> construct_hypergraphs_from_files(string db_file_path, string info_file_path, bool safe) {
    vector<HyperGraph> constructed_hypergraphs;
    HyperGraph original_hypergraph(db_file_path, info_file_path, safe);
    if (original_hypergraph.is_connected()) {
        constructed_hypergraphs.emplace_back(original_hypergraph);
        return constructed_hypergraphs;
    } else {
        vector<set<NodeId>> unconnected_components = original_hypergraph.find_unconnected_components();
        for (auto component: unconnected_components) {
            HyperGraph subgraph(component, original_hypergraph);
            constructed_hypergraphs.emplace_back(subgraph);
        }
        return constructed_hypergraphs;
    }
}



