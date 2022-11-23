//
// Created by jonathan on 19/03/2022.
//

#ifndef FASTER_HYPERGRAPH_H
#define FASTER_HYPERGRAPH_H

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <unordered_set>
#include <string>
#include <cctype>
#include <list>
#include <algorithm>
#include <random>
#include "map_utils.h"
#include "UndirectedGraph.h"
#include <fstream>
#include "random_utils.h"
#include "read_files_utils.h"
#include "Relation.h"
#include "exceptions.h"
#include <error.h>
#include "set_utils.h"

using namespace std;
class UndirectedGraph;
using NodeId = size_t;
using NodeName = string;
using EdgeId = size_t;
using Predicate = string;
using NodeType = string;


class HyperGraph{
private:
    // MEMBERS
    map<NodeId, set<Predicate>> singleton_edges; // node_id : set(predicate)
    map<EdgeId, vector<NodeId>> edges; // edge_id : list(node_id)
    map<EdgeId, double> edge_weights; // edge_id : weight
    map<EdgeId, Predicate> predicates; // edge_id : predicate_name
    map<NodeId, NodeName> node_ids_names; // node_id : node_name
    map<NodeName, NodeId> node_names_ids; // node_name : node_id
    map<NodeId, NodeType> nodes; // node_id : node_type
    map<NodeId, vector<EdgeId>> memberships; // node_id : list(edge_id)
    map<Predicate, vector<NodeType>> predicate_argument_types; // predicate_name : list(node_type)
    set<NodeType> node_types; // set(node_type)
    size_t estimated_graph_diameter{0};
    map<NodeId, bool> is_source_node; // node_id : bool

    //METHODS
    void set_predicate_argument_types_from_file(string const& info_file_path, bool safe);

public:
    HyperGraph();
    HyperGraph(string const& db_file_path, string const& info_file_path, bool safe);
    HyperGraph(UndirectedGraph &graph, HyperGraph &hypergraph_template);
    HyperGraph(set<NodeId> nodes_subset, HyperGraph &hypergraph_template);

    ~HyperGraph();

    bool is_connected();
    bool check_is_source_node(NodeId node_id);
    void add_edge(EdgeId edge_id, Predicate const& predicate, vector<NodeId> node_ids, double weight);
    void add_edge(Predicate const& predicate, NodeId node_id);
    map<NodeId, set<Predicate>> get_singleton_edges();
    set<NodeId> get_node_ids();
    map<EdgeId, vector<NodeId>>& get_edges();
    vector<NodeId> get_edge(EdgeId edge_id);
    map<NodeId, NodeType> get_nodes();
    vector<NodeId> get_nodes_of_edge(EdgeId edge_id);
    string_view get_predicate(EdgeId edge_id);
    set<NodeType> get_node_types();
    map<NodeId, vector<EdgeId>> get_memberships();
    vector<EdgeId> get_memberships(NodeId node_id);
    size_t number_of_nodes(); //Number of nodes
    size_t number_of_edges(); //Number of edges
    int number_of_predicates();
    size_t get_estimated_graph_diameter() const;
    pair<EdgeId, NodeId> get_random_edge_and_neighbor_of_node(NodeId const& node);
    map<Predicate, vector<NodeType>> get_predicate_argument_types();
    vector<NodeType> get_predicate_argument_types(Predicate &predicate);
    map<NodeId, NodeName> get_node_ids_names();
    map<NodeName, NodeId> get_node_names_ids();
    double get_edge_weight(EdgeId edge_id);
    void compute_diameter();
    void print();
    vector<set<NodeId>> find_unconnected_components();
    set<NodeId> dfs(NodeId source_node);
};


vector<HyperGraph> construct_hypergraphs_from_files(string db_file_path, string info_file_path, bool safe);


#endif //FASTER_HYPERGRAPH_H
