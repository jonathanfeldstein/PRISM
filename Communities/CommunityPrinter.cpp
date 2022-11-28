#include "CommunityPrinter.h"

#include <utility>
using namespace std;

CommunityPrinter::CommunityPrinter(vector<Communities> communities_vector, HyperGraph original_hypergraph) {
    this->communities_vector = std::move(communities_vector);
    for(auto &communities:this->communities_vector){
        this->number_of_communities += communities.size();
    }

    if(this->number_of_communities > original_hypergraph.number_of_nodes()){
        throw CommunitiesGenerationException(this->number_of_communities, original_hypergraph.number_of_nodes());
    }

    // .ldb is a type of file used by Alchemy. This maps each node to its string representation for the ldb file.
    this->get_node_to_ldb_string_map();
}

CommunityPrinter::~CommunityPrinter() = default;

void CommunityPrinter::write_ldb_file(string filename) {
    filename += ".ldb";
    ofstream file;
    file.open(filename);
    if(file.is_open()){
        this->write_header(file);
        HyperGraphId temp_hypergraph_id{0};
        CommunityId temp_community_id{0};

        for(auto communities: this->communities_vector){
            this->hypergraph_id = temp_hypergraph_id;
            for (auto& community:get_values(communities.get_communities())) {
                this->community_id = temp_community_id;
                string string_type = "ldb";
                set<Atom> ldb_atoms = get_atoms_of_community(community, communities.get_hypergraph(), string_type);
                this->write_atoms_to_file(ldb_atoms, file);
                temp_community_id++;
            }
            temp_hypergraph_id++;
        }
        this->write_footer(file);
    }else{
        throw FileNotOpenedException(filename);
    }
}

void CommunityPrinter::write_uldb_file(string filename) {
    filename += ".uldb";
    ofstream file;
    file.open(filename);
    if(file.is_open()){
        this->write_header(file);
        HyperGraphId temp_hypergraph_id{0};
        CommunityId temp_community_id{0};

        for(auto communities: this->communities_vector){
            this->hypergraph_id = temp_hypergraph_id;
            for (auto& community:get_values(communities.get_communities())) {
                this->community_id = temp_community_id;
                string string_type = "uldb";
                set<Atom> uldb_atoms = get_atoms_of_community(community, communities.get_hypergraph(), string_type);
                this->write_atoms_to_file(uldb_atoms, file);
                temp_community_id++;
            }
            temp_hypergraph_id++;
        }
        this->write_footer(file);
    }else{
        throw FileNotOpenedException(filename);
    }

}

void CommunityPrinter::write_srcnclust_file(string filename) {
    filename += ".srcnclusts";
    ofstream file;
    file.open(filename);
    if(file.is_open()){
        this->write_header(file);
        HyperGraphId temp_hypergraph_id{0};
        for(auto communities: this->communities_vector){
            this->hypergraph_id = temp_hypergraph_id;
            CommunityId temp_community_id{0};
            for (auto& community:get_values(communities.get_communities())) {
                this->community_id = temp_community_id;
                NodePartition node_partition = get_node_ids(community);
                this->write_community_source_node_to_file(community, file);
                this->write_single_node_ids_to_file(node_partition.single_nodes, file);
                this->write_cluster_node_ids_to_file(node_partition.clusters, file);
                this->write_all_node_ids_to_file(node_partition.single_nodes, node_partition.clusters, file);
                temp_community_id++;
            }
            temp_hypergraph_id++;
        }
        this->write_footer(file);
    }else{
        throw FileNotOpenedException(filename);
    }

}

void CommunityPrinter::write_header(ofstream &file) {
    file <<"#START_GRAPH  #COMS "<<this->number_of_communities<<endl<<endl;
}

void CommunityPrinter::write_atoms_to_file(set<string> &atoms, ofstream &file) {
    file << "#START_DB  " << this->community_id << "  #COMS  1  #NUM_ATOMS " << atoms.size() << endl;
    for(auto &atom: atoms){
        file << atom << endl;
    }
    file << "#END_DB" << endl << endl;
}

void CommunityPrinter::write_footer(ofstream &file) {
    file<<"#END_GRAPH"<<endl;
}

void CommunityPrinter::write_community_source_node_to_file(Community &community, ofstream &file) {
    file << "#START_DB  " << this->community_id;
    file << "  #NUM_SINGLES " << community.number_of_single_nodes ;
    file << "  #NUM_CLUSTS " << community.number_of_clusters ;
    file << "  #NUM_NODES " << community.number_of_nodes << endl;
    file << "SRC " << community.source_node << endl;
}

void CommunityPrinter::write_single_node_ids_to_file(set<NodeId> &node_ids, ofstream &file) {
    for(auto node_id: node_ids){
        file << node_id << endl;
    }
}

void CommunityPrinter::write_cluster_node_ids_to_file(vector<Cluster> &cluster_node_ids, ofstream &file) {
    size_t cluster_id{0};
    for(auto node_ids: cluster_node_ids){
        stringstream string_of_node_ids;
        copy(node_ids.begin(), node_ids.end(), std::ostream_iterator<NodeId>(string_of_node_ids, " "));
        file << "CLUST " << cluster_id << "  " << string_of_node_ids.str() << endl;
        cluster_id++;
    }

}

void CommunityPrinter::write_all_node_ids_to_file(set<NodeId> &single_node_ids, vector<Cluster> &cluster_node_ids, ofstream &file) {
    vector<NodeId> flattened_cluster_node_ids;
    for (auto cluster: cluster_node_ids){
        flattened_cluster_node_ids.insert(flattened_cluster_node_ids.end(), cluster.begin(), cluster.end());
    }
    vector<NodeId> all_node_ids(single_node_ids.begin(), single_node_ids.end());
    all_node_ids.insert(all_node_ids.end(), flattened_cluster_node_ids.begin(), flattened_cluster_node_ids.end());
    sort(all_node_ids);
    stringstream all_node_ids_string;
    copy(all_node_ids.begin(), all_node_ids.end(), std::ostream_iterator<size_t>(all_node_ids_string, " "));
    file << "NODES  " << all_node_ids_string.str() << endl;
    file << "#END_DB" << endl << endl;
}

set<string> CommunityPrinter::get_atoms_of_community(Community &community,
                                                     HyperGraph &hypergraph_of_community,
                                                     string &string_type) {
    set<Atom> atoms;

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

set<Atom> CommunityPrinter::get_atoms_of_node_in_community(NodeId node,
                                                             Community &community,
                                                             HyperGraph &hypergraph_of_community,
                                                             string &string_type) {
    set<Atom> atoms;
    vector<EdgeId> non_singleton_edges = hypergraph_of_community.get_memberships(node);
    for(auto edge: non_singleton_edges){
        vector<NodeId> nodes_of_edge = hypergraph_of_community.get_nodes_of_edge(edge);
        set<NodeId> nodes_of_edge_set(nodes_of_edge.begin(), nodes_of_edge.end());
        if(includes(community.nodes.begin(), community.nodes.end(), nodes_of_edge_set.begin(), nodes_of_edge_set.end())){
            string predicate = hypergraph_of_community.get_predicate(edge).data();
            atoms.insert(this->get_atom_for_edge(predicate, nodes_of_edge, string_type));
        }else{
            continue;
        }
    }
    for(auto &singleton_edges: hypergraph_of_community.get_singleton_edges()){
        if(community.nodes.find(singleton_edges.first) != community.nodes.end()){
            for(auto &predicate: singleton_edges.second){
                vector<NodeId> singleton;
                singleton.emplace_back(singleton_edges.first);
                atoms.insert(this->get_atom_for_edge(predicate, singleton, string_type));
            }
        }
    }
    return atoms;
}

Atom CommunityPrinter::get_atom_for_edge(const Predicate &edge_predicate, vector<NodeId> &nodes_of_edge, string &string_type) {
    Atom atom = edge_predicate + "(";
    vector<NodeName> node_names = get_node_names(nodes_of_edge, string_type);
    atom += boost::algorithm::join(node_names, ",") + ")";
    return atom;
}

vector<NodeName> CommunityPrinter::get_node_names(vector<size_t> &nodes_of_edge, string &string_type) {
    vector<NodeName> node_names;
    if(string_type == "ldb"){
        for(auto node:nodes_of_edge){
            node_names.emplace_back(this->node_to_ldb_string[this->hypergraph_id][this->community_id][node]);
        }
        return node_names;
    }else {
        for(auto node:nodes_of_edge){
            node_names.emplace_back("NODE_"+to_string(node));
        }
        return node_names;
    }
}

NodePartition CommunityPrinter::get_node_ids(Community &community) {
    NodePartition node_partition;
    node_partition.single_nodes = community.single_nodes;
    node_partition.clusters = community.clusters;
    return node_partition;
}

void CommunityPrinter::get_node_to_ldb_string_map() {
    HyperGraphId temp_hypergraph_id{0};
    CommunityId temp_community_id{0};

    for(auto communities:this->communities_vector){
        for(const auto &community: get_values(communities.get_communities())){
            for(auto single_node: community.single_nodes){
                this->node_to_ldb_string[temp_hypergraph_id][temp_community_id][single_node] = "NODE_" + to_string(single_node);
            }
            size_t cluster_id{0};
            for(const auto &cluster: community.clusters){
                for(auto cluster_node : cluster){
                    this->node_to_ldb_string[temp_hypergraph_id][temp_community_id][cluster_node] = "CLUST_" + to_string(cluster_id);
                }
                cluster_id++;
            }
            temp_community_id++;
        }
        temp_hypergraph_id++;
    }
}

void CommunityPrinter::write_files(string &filename) {
    this->write_ldb_file(filename);
    this->write_uldb_file(filename);
    this->write_srcnclust_file(filename);

}
