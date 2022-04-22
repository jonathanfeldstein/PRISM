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

class CommunityPrinter{
private:
    vector<Communities> communities_vector;
    size_t number_of_communities{0};
    map<size_t, map<size_t, map<size_t, string>>> node_to_ldb_string;
    size_t hypergraph_number{0};
    size_t community_number{0};

    void write_ldb_file(string filename);
    void write_uldb_file(string filename);
    void write_srcnclust_file(string filename);
    void write_header(ofstream &file);
    void write_atoms_to_file(set<string> &atoms, ofstream &file);
    void write_footer(ofstream &file);
    void write_community_source_node_to_file(Community &community, ofstream &file);
    void write_single_node_ids_to_file(vector<size_t> &node_id_vector, ofstream &file);
    void write_cluster_node_ids_to_file(vector<vector<size_t>> &cluster_node_ids, ofstream &file);
    void write_all_node_ids_to_file(vector<size_t> &single_node_ids, vector<vector<size_t>> &cluster_node_ids, ofstream &file);

    set<string> get_atoms_of_community(Community &community,
                                       HyperGraph &hypergraph_of_community,
                                       string string_type);

    set<string> get_atoms_of_node_in_community(size_t node,
                                               Community &community,
                                               HyperGraph &hypergraph_of_community,
                                               string string_type);

    string get_atom_for_edge(string edge_predicate, vector<size_t> &nodes_of_edge, string string_type);
    vector<string> get_node_names(vector<size_t> &nodes_of_edge, string string_type);
    pair<vector<size_t>, vector<vector<size_t>>> get_node_ids(Community &community);
    void get_node_to_ldb_string_map();

public:
    CommunityPrinter(vector<Communities> communities, HyperGraph original_hypergraph);
    ~CommunityPrinter();
    void write_files(string filename);

};
#endif //FASTER_COMMUNITYPRINTER_H
