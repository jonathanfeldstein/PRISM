#ifndef PRISM_HYPERGRAPH_H
#define PRISM_HYPERGRAPH_H

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
#include "../Utils/map_utils.h"
#include "UndirectedGraph.h"
#include <fstream>
#include "../Utils/random_utils.h"
#include "../Utils/read_files_utils.h"
#include "Relation.h"
#include "../Utils/exceptions.h"
#include <error.h>
#include "../Utils/set_utils.h"
#include "../Utils/vector_utils.h"

using namespace std;
class UndirectedGraph;
using NodeId = size_t;
using NodeName = string;
using EdgeId = size_t;
using Predicate = string;
using NodeType = string;
using UnaryPattern = set<int>;
using BinaryPattern = vector<UnaryPattern>;
using TernaryPattern = vector<UnaryPattern>;
using TernaryCliquePattern = vector<UnaryPattern>;
using QuaternaryPattern = vector<UnaryPattern>;


class HyperGraph{
private:
    // MEMBERS
    map<EdgeId, vector<NodeId>> edges; // edge_id : list(node_id)
    map<EdgeId, Predicate> predicates; // edge_id : predicate_name
    // node_id : node_type
    map<NodeId, vector<EdgeId>> memberships; // node_id : list(edge_id)
    // set(node_type)
    size_t estimated_graph_diameter{0};
    map<NodeId, bool> is_source_node; // node_id : bool

    //METHODS
    void set_predicate_argument_types_from_file(string const& info_file_path, bool safe);

protected:
    map<Predicate, vector<NodeType>> predicate_argument_types; // predicate_name : list(node_type)
    map<EdgeId, double> edge_weights; // edge_id : weight
    map<NodeId, NodeName> node_ids_names; // node_id : node_name
    map<NodeName, NodeId> node_names_ids;
    set<NodeType> node_types; // node_name : node_id
    map<NodeId, NodeType> nodes;


//    vector<UnaryPattern> translate_pattern(vector<UnaryPattern> original){
//        vector<UnaryPattern> translated;
//        for(auto section:original){
//            UnaryPattern translated_section;
//            for(auto edge: section){
//                int predicate_id = predicate_to_id.at(get_predicate(edge).data());
//                translated_section.insert(predicate_id);
//            }
//            translated.push_back(translated_section);
//        }
//        return translated;
//    }

public:
    HyperGraph();
    HyperGraph(string const& db_file_path, string const& info_file_path, bool safe);
    HyperGraph(UndirectedGraph &graph, HyperGraph &hypergraph_template);
    HyperGraph(set<NodeId> nodes_subset, HyperGraph &hypergraph_template);

    ~HyperGraph();

    bool is_connected();
    bool check_is_source_node(NodeId node_id);
    void add_edge(EdgeId edge_id, Predicate const& predicate, vector<NodeId> node_ids, double weight);
    set<NodeId> get_node_ids();
    map<EdgeId, vector<NodeId>>& get_edges();
    vector<NodeId> get_edge(EdgeId edge_id);
    map<NodeId, NodeType> get_nodes();
    vector<NodeId> get_nodes_of_edge(EdgeId edge_id);
    string_view get_predicate(EdgeId edge_id);
    map<EdgeId, Predicate> get_predicates();
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

//    void find_paths(map<vector<UnaryPattern>,vector<UnaryPattern>> &unary_patterns,
//                    map<vector<UnaryPattern>,vector<vector<UnaryPattern>>> &binary_patterns,
//                    map<vector<UnaryPattern>,vector<vector<UnaryPattern>>> &ternary_patterns,
//                    map<vector<UnaryPattern>,vector<vector<UnaryPattern>>> &ternary_clique_patterns,
//                    map<vector<UnaryPattern>,vector<vector<UnaryPattern>>> &quaternary_patterns) {
//        int predicate_count=1;
//        vector<Predicate> predicate_vector = get_values(this->predicates);
//        for(auto predicate : predicate_vector){
//            if(!has(get_keys(predicate_to_id), predicate)){
//                predicate_to_id[predicate] = predicate_count;
//                predicate_count++;
//            }
//        }
//        //Find all sub-patterns
//        map<NodeId,map<NodeId, UnaryPattern>> sub_patterns;
//        for(auto node : get_keys(get_nodes())){
//            sub_patterns[node] = find_next_nodes_and_edges(node);
//        }
//        for(auto node_i : get_keys(sub_patterns)){
//            if (sub_patterns[node_i][node_i].size() > 1){ // Only care about unary patterns with at least 2 edges
//                UnaryPattern unary_pattern = sub_patterns[node_i][node_i];
//                unary_patterns[translate_pattern(vector<UnaryPattern>{unary_pattern})].push_back(unary_pattern);
//            }
//            for(auto node_j : get_keys(sub_patterns[node_i])){
//                if(node_j != node_i){
//                    BinaryPattern binary_pattern = {sub_patterns[node_i][node_i],sub_patterns[node_i][node_j],sub_patterns[node_j][node_j]};
//                    binary_patterns[translate_pattern(binary_pattern)].push_back(binary_pattern);
//                    for(auto node_k : get_keys(sub_patterns[node_j])){
//                        if(node_k != node_j && node_k != node_i){
//                            TernaryPattern ternary_pattern = {sub_patterns[node_i][node_i],
//                                                              sub_patterns[node_i][node_j],
//                                                              sub_patterns[node_j][node_j],
//                                                              sub_patterns[node_j][node_k],
//                                                              sub_patterns[node_k][node_k]};
//                            ternary_patterns[translate_pattern(ternary_pattern)].push_back(ternary_pattern);
//                            for(auto node_l : get_keys(sub_patterns[node_k])){
//                                if(node_l != node_k && node_l != node_j){
//                                    if(node_l != node_i){
//                                        QuaternaryPattern quaternary_pattern = {sub_patterns[node_i][node_i],
//                                                                                sub_patterns[node_i][node_j],
//                                                                                sub_patterns[node_j][node_j],
//                                                                                sub_patterns[node_j][node_k],
//                                                                                sub_patterns[node_k][node_k],
//                                                                                sub_patterns[node_k][node_l],
//                                                                                sub_patterns[node_l][node_l]};
//                                        quaternary_patterns[translate_pattern(quaternary_pattern)].push_back(
//                                                    quaternary_pattern);
//                                    }else{
//                                        TernaryCliquePattern ternary_clique_pattern = {sub_patterns[node_i][node_i],
//                                                                                       sub_patterns[node_i][node_j],
//                                                                                       sub_patterns[node_j][node_j],
//                                                                                       sub_patterns[node_j][node_k],
//                                                                                       sub_patterns[node_k][node_k],
//                                                                                       sub_patterns[node_k][node_i]};
//                                        ternary_clique_patterns[translate_pattern(
//                                                    ternary_clique_pattern)].push_back(ternary_clique_pattern);
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
    map<NodeId,UnaryPattern> find_next_nodes_and_edges(NodeId current_node){
        vector<EdgeId> connected_edges = get_memberships(current_node);
        map<NodeId, UnaryPattern> next_nodes_and_edges;
        for(auto edge:connected_edges){
            set<NodeId> next_nodes  = to_set(get_nodes_of_edge(edge));
            if(next_nodes.size()==1){
                next_nodes_and_edges[current_node].insert(edge);
            }else if(next_nodes.size()== 2){
                for(auto node:next_nodes){
                    if(node != current_node){
                        next_nodes_and_edges[node].insert(edge);
                    }
                }
            }else{
                throw HyperGraphNotImplementedException();
            }
        }
        return next_nodes_and_edges;
    }
};


vector<HyperGraph> construct_hypergraphs_from_files(string db_file_path, string info_file_path, bool safe);


#endif //PRISM_HYPERGRAPH_H
