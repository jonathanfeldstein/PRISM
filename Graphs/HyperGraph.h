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

using namespace std;
class UndirectedGraph;



class HyperGraph{
private:
    map<size_t, set<string>> singleton_edges; // node_id : set(predicate)
    map<size_t, vector<size_t>> edges; // edge_id : list(node_id)
    map<size_t, string> predicates; // edge_id : predicate_name
    map<size_t, string> node_ids_names; // node_id : node_name
    map<string, size_t> node_names_ids; // node_name : node_id
    map<size_t, string> nodes; // node_id : node_type
    map<size_t, set<size_t>> memberships; // node_id : set(edge_id)
    map<string, vector<string>> predicate_argument_types; // predicate_name : list(node_type)
    set<string> node_types; // set(node_type)
    double estimated_graph_diameter;
    map<size_t, bool> is_source_node; // node_id : bool
    void set_predicate_argument_types_from_file(string const& info_file_path);
    pair<string, vector<string>> parse_line(string line);

public:
    HyperGraph();
    HyperGraph(string const& db_file_path, string const& info_file_path);
    HyperGraph(UndirectedGraph &graph, HyperGraph &hypergraph_template);

    ~HyperGraph();

    bool is_connected();
    bool check_is_source_node(int node_id);
    void add_edge(size_t edge_id, string const& predicate, vector<size_t>node_ids);
    void add_edge(string const& predicate, size_t node_id);
    map<size_t, set<string>> get_singleton_edges();
    set<size_t> get_node_ids();
    map<size_t, vector<size_t>> get_edges();
    map<size_t, string> get_nodes(); // TODO think about nomenclature
    vector<size_t> get_nodes_of_edge(size_t edge_id);
    map<size_t, string> get_predicates();
    string get_predicate(size_t edge_id);
    set<string> get_node_types();
    map<size_t, set<size_t>> get_memberships();
    set<size_t> get_memberships(size_t node_id);
    size_t number_of_nodes(); //Number of nodes
    size_t number_of_edges(); //Number of edges
    int number_of_predicates();
    double get_estimated_graph_diameter();
    pair<size_t, size_t> get_random_edge_and_neighbor_of_node(size_t const& node);
    map<string, vector<string>> get_predicate_argument_types();
    vector<string> get_predicate_argument_types(string predicate);
    map<size_t, string> get_node_ids_names();
    void print();
};
#endif //FASTER_HYPERGRAPH_H
