//
// Created by jonathan on 21/03/2022.
//

#include "CommunityPrinter.h"

#include <utility>
using namespace std;

CommunityPrinter::CommunityPrinter(vector<Communities> communities, HyperGraph original_hypergraph) {
    this->communities_vector = std::move(communities);
    for(auto communities:communities_vector){
        this->number_of_communities += communities.size();
    }

    //TODO needs to break if true
    if(this->number_of_communities <= original_hypergraph.number_of_nodes()){
        cout<<"Incorrect hypergraph provided for original_hypergraph. More communities found (";
        cout<<this->number_of_communities;
        cout<<") than the number of node in original_hypergraph (";
        cout<<original_hypergraph.number_of_nodes()<<").";
    }

    // .ldb is a type of file used by Alchemy. This maps each node to its string representation for the ldb file.
    this->get_node_to_ldb_string_map();
}

CommunityPrinter::~CommunityPrinter() = default;

void CommunityPrinter::write_ldb_file(string filename) {
    filename += ".ldb";
    ofstream file;
    file.open(filename, std::ios_base::app);
    this->write_header(file);
    size_t hypergraph_number_temp{0};
    for(auto communities: this->communities_vector){
        this->hypergraph_number = hypergraph_number_temp;
        size_t community_number_temp{0};
        for (auto& community:get_values(communities.get_communities())) {
            this->community_number = community_number_temp;
            set<string> ldb_atoms = get_atoms_of_community(community, communities.get_hypergraph(), "ldb");
            this->write_atoms_to_file(ldb_atoms, file);
            community_number_temp++;
        }
        hypergraph_number_temp++;
    }
    this->write_footer(file);
}

void CommunityPrinter::write_uldb_file(string filename) {
    filename += ".uldb";
    ofstream file;
    file.open(filename, std::ios_base::app);
    this->write_header(file);
    size_t hypergraph_number_temp{0};
    for(auto communities: this->communities_vector){
        this->hypergraph_number = hypergraph_number_temp;
        size_t community_number_temp{0};
        for (auto& community:get_values(communities.get_communities())) {
            this->community_number = community_number_temp;
            set<string> uldb_atoms = get_atoms_of_community(community, communities.get_hypergraph(), "uldb");
            this->write_atoms_to_file(uldb_atoms, file);
            community_number_temp++;
        }
        hypergraph_number_temp++;
    }
    this->write_footer(file);
}

void CommunityPrinter::write_srcnclust_file(string filename) {
    filename += ".srcnclusts";
    ofstream file;
    file.open(filename, std::ios_base::app);
    this->write_header(file);
    size_t hypergraph_number_temp{0};
    for(auto communities: this->communities_vector){
        this->hypergraph_number = hypergraph_number_temp;
        size_t community_number_temp{0};
        for (auto& community:get_values(communities.get_communities())) {
            this->community_number = community_number_temp;
            pair<vector<size_t>, vector<vector<size_t>>> single_and_cluster_node_ids = get_node_ids(community);
            this->write_community_source_node_to_file(community, file);
            this->write_single_node_ids_to_file(single_and_cluster_node_ids.first, file);
            this->write_cluster_node_ids_to_file(single_and_cluster_node_ids.second, file);
            this->write_all_node_ids_to_file(single_and_cluster_node_ids.first, single_and_cluster_node_ids.second, file);
            community_number_temp++;
        }
        hypergraph_number_temp++;
    }
    this->write_footer(file);
}

void CommunityPrinter::write_header(ofstream &file) {
    file <<"#START_GRAPH  #COMS "<<this->number_of_communities<<endl<<endl;
}

void CommunityPrinter::write_atoms_to_file(set<string> &atoms, ofstream &file) {
    file << "#START_DB " << this->community_number << " #COM 1 #NUM_ATOMS " << atoms.size() << " " << endl;
    for(auto &atom: atoms){
        file << atom << endl;
    }
    file << "#END_DB" << endl << endl;
}

void CommunityPrinter::write_footer(ofstream &file) {
    file<<"#END_Graph"<<endl;
}

void CommunityPrinter::write_community_source_node_to_file(Community &community, ofstream &file) {
    file << "#START_DB " << this->community_number;
    file << " #NUM_SINGLES " << community.number_of_single_nodes ;
    file << " #NUM_CLUSTS " << community.number_of_clusters ;
    file << " #NUM_NODES " << community.number_of_nodes << endl;
    file << "SRC " << community.source_node << endl;
}

void CommunityPrinter::write_single_node_ids_to_file(vector<size_t> &node_id_vector, ofstream &file) {
    for(auto node_id: node_id_vector){
        file << node_id << endl;
    }
}

void CommunityPrinter::write_cluster_node_ids_to_file(vector<vector<size_t>> &cluster_node_ids, ofstream &file) {
    size_t cluster_id{0};
    size_t idx{0};
    for(auto node_id_vector: cluster_node_ids){
        stringstream string_of_node_ids;
        copy(node_id_vector.begin(), node_id_vector.end(), std::ostream_iterator<size_t>(string_of_node_ids, " "));
        file << "CLUST " << idx << " " << string_of_node_ids.str() << endl;
    }

}

void CommunityPrinter::write_all_node_ids_to_file(vector<size_t> &single_node_ids, vector<vector<size_t>> &cluster_node_ids, ofstream &file) {
    vector<size_t> flattened_cluster_node_ids = flatten(cluster_node_ids);
    vector<size_t> all_node_ids(single_node_ids.begin(), single_node_ids.end());
    all_node_ids.insert(all_node_ids.end(), flattened_cluster_node_ids.begin(), flattened_cluster_node_ids.end());
    sort(all_node_ids);
    stringstream all_node_ids_string;
    copy(all_node_ids.begin(), all_node_ids.end(), std::ostream_iterator<size_t>(all_node_ids_string, " "));
    file << "NODES " << all_node_ids_string.str() << endl;
    file << "#END_DB" << endl << endl;
}

set<string> CommunityPrinter::get_atoms_of_community(Community &community,
                                                     HyperGraph &hypergraph_of_community,
                                                     string string_type) {
    set<string> atoms;

    for(auto single_node: community.single_nodes){
        atoms.merge(this->get_atoms_of_node_in_community(single_node,
                                                                community,
                                                                hypergraph_of_community,
                                                                string_type));
    }
    for(auto &cluster:community.clusters){
        for(auto cluster_node: cluster){
            atoms.merge(this->get_atoms_of_node_in_community(cluster_node,
                                                                    community,
                                                                    hypergraph_of_community,
                                                                    string_type));
        }
    }
    return atoms;
}

set<string> CommunityPrinter::get_atoms_of_node_in_community(size_t node,
                                                             Community &community,
                                                             HyperGraph &hypergraph_of_community,
                                                             string string_type) {
    set<string> atoms;
    vector<size_t> non_singleton_edges = hypergraph_of_community.get_memberships(node);
    for(auto edge: non_singleton_edges){
        vector<size_t> nodes_of_edge = hypergraph_of_community.get_nodes_of_edge(edge);
        set<size_t> nodes_of_edge_set(nodes_of_edge.begin(), nodes_of_edge.end());
        if(includes(community.nodes.begin(), community.nodes.end(), nodes_of_edge_set.begin(), nodes_of_edge_set.end())){
            string predicate = hypergraph_of_community.get_predicate(edge);
            atoms.insert(this->get_atom_for_edge(predicate, nodes_of_edge, string_type));
        }else{
            continue;
        }
    }
    for(auto &singleton_edges: hypergraph_of_community.get_singleton_edges()){
        if(community.nodes.find(singleton_edges.first) != community.nodes.end()){
            for(auto &predicate: singleton_edges.second){
                vector<size_t> singleton(singleton_edges.first);
                atoms.insert(this->get_atom_for_edge(predicate, singleton, string_type));
            }
        }
    }
    return atoms;
}

string CommunityPrinter::get_atom_for_edge(string edge_predicate, vector<size_t> &nodes_of_edge, string string_type) {
    string atom = edge_predicate + "(";
    vector<string> node_names = get_node_names(nodes_of_edge, string_type);
    edge_predicate += boost::algorithm::join(node_names, ",") + ")";
    return atom;
}

vector<string> CommunityPrinter::get_node_names(vector<size_t> &nodes_of_edge, string string_type) {
    vector<string> node_names;
    if(string_type == "ldb"){
        for(auto node:nodes_of_edge){
            node_names.emplace_back(this->node_to_ldb_string[this->hypergraph_number][this->community_number][node]);
        }
        return node_names;
    }else if(string_type == "uldb"){
        for(auto node:nodes_of_edge){
            node_names.emplace_back("NODE_"+to_string(node));
        }
        return node_names;
    }else{
        cout<<"String types other than 'ldb' or 'uldb' are not supported."<<endl; // TODO should throw error.
        return node_names;
    }
}

pair<vector<size_t>, vector<vector<size_t>>> CommunityPrinter::get_node_ids(Community &community) {
    vector<size_t> single_node_ids(community.single_nodes.begin(), community.single_nodes.end());
    sort(single_node_ids);
    vector<vector<size_t>> cluster_node_ids;
    for(auto cluster: community.clusters){
        vector<size_t> node_ids(cluster.begin(), cluster.end());
        sort(node_ids);
        cluster_node_ids.emplace_back(node_ids);
    }
    return {single_node_ids, cluster_node_ids};
}

void CommunityPrinter::get_node_to_ldb_string_map() {
    size_t hypergraph_id{0};
    for(auto communities:this->communities_vector){
        size_t community_id{0};
        for(const auto &community: get_values(communities.get_communities())){
            for(auto single_node: community.single_nodes){
                this->node_to_ldb_string[hypergraph_id][community_id][single_node] = "NODE_" + to_string(single_node);
            }
            size_t cluster_id{0};
            for(const auto &cluster: community.clusters){
                for(auto cluster_node : cluster){
                    this->node_to_ldb_string[hypergraph_id][community_id][cluster_node] = "CLUST_" + to_string(cluster_id);
                }
                cluster_id++;
            }
            community_id++;
        }
        hypergraph_id++;
    }
}

void CommunityPrinter::write_files(string filename) {
    this->write_ldb_file(filename);
    this->write_uldb_file(filename);
    this->write_srcnclust_file(filename);

}
