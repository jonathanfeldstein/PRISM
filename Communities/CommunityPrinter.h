//
// Created by jonathan on 21/03/2022.
//

#ifndef FASTER_COMMUNITYPRINTER_H
#define FASTER_COMMUNITYPRINTER_H

#include <map>
#include <string>
#include "Communities.h"

using namespace std;

class CommunityPrinter{
private:
    list<Communities> communities_list;
    size_t number_of_communities;
    map<string, int> node_to_node_id;
    map<int, string> node_to_ldb_string; // TODO Int for nodes???
    size_t hypergraph_number;
    size_t community_number;
    void write_ldb_file();
    void write_uldb_file();
    void write_srcnclust_file();
    void write_header();
    void write_atoms_to_file();
    void write_footer();
    void write_community_source_node_to_file();
    void write_single_node_ids_to_file();
    void write_cluster_node_ids_to_file();
    void write_all_node_ids_to_file();
    void get_atoms_of_community();
    void get_atoms_of_node_in_community();
    void get_atom_for_edge();
    void get_node_name();
    void get_node_ids(Community community);
    void get_node_to_ldb_string_map();

public:
    CommunityPrinter();
    ~CommunityPrinter();
    void write_files();

};
#endif //FASTER_COMMUNITYPRINTER_H
