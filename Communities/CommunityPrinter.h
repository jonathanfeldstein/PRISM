//
// Created by jonathan on 21/03/2022.
//

#ifndef FASTER_COMMUNITYPRINTER_H
#define FASTER_COMMUNITYPRINTER_H

#include <map>
#include <string>
#include "Communities.h"
#include <boost/algorithm/string/join.hpp>
#include "../Utils/vector_utils.h"

using namespace std;
using NodeId = size_t;
using EdgeId = size_t;
using Predicate = string;
using HyperGraphId = size_t;
using CommunityId = size_t;
using Atom = string;
using NodeName = string;

class CommunityPrinter{
private:
    vector<Communities> communities_vector;
    size_t number_of_communities{0};
    map<HyperGraphId, map<CommunityId, map<NodeId, NodeName>>> node_to_ldb_string;
    HyperGraphId hypergraph_id{0}; // TODO hypergraph_id vs hypergraph_number
    CommunityId community_id{0};

    void write_ldb_file(string filename);
    void write_uldb_file(string filename);
    void write_srcnclust_file(string filename);
    void write_header(ofstream &file);
    void write_atoms_to_file(set<string> &atoms, ofstream &file);
    void write_footer(ofstream &file);
    void write_community_source_node_to_file(Community &community, ofstream &file);
    void write_single_node_ids_to_file(vector<NodeId> &node_ids, ofstream &file);
    void write_cluster_node_ids_to_file(vector<vector<NodeId>> &cluster_node_ids, ofstream &file);
    void write_all_node_ids_to_file(vector<NodeId> &single_node_ids, vector<vector<NodeId>> &cluster_node_ids, ofstream &file); // TODO Check if we can just use set<NodeId> as it should be sorted... -> Cluster = set<NodeId>

    set<Atom> get_atoms_of_community(Community &community,
                                     HyperGraph &hypergraph_of_community,
                                     string &string_type);

    set<Atom> get_atoms_of_node_in_community(NodeId node,
                                             Community &community,
                                             HyperGraph &hypergraph_of_community,
                                             string &string_type);

    Atom get_atom_for_edge(const Predicate &edge_predicate,
                           vector<NodeId> &nodes_of_edge,
                           string &string_type);

    vector<NodeName> get_node_names(vector<NodeId> &nodes_of_edge,
                                    string &string_type);

    pair<vector<size_t>, vector<vector<size_t>>> get_node_ids(Community &community);
    void get_node_to_ldb_string_map();

public:
    CommunityPrinter(vector<Communities> communities, HyperGraph original_hypergraph);
    ~CommunityPrinter();
    void write_files(string &filename);

};
#endif //FASTER_COMMUNITYPRINTER_H
