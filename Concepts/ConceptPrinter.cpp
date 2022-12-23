#include "ConceptPrinter.h"

#include <utility>
using namespace std;

ConceptPrinter::ConceptPrinter(vector<Concepts> concepts_vector, HyperGraph original_hypergraph) {
    this->concepts_vector = std::move(concepts_vector);
    for(auto &concepts:this->concepts_vector){
        this->number_of_concepts += concepts.size();
    }

    if(this->number_of_concepts > original_hypergraph.number_of_nodes()){
        throw ConceptsGenerationException(this->number_of_concepts, original_hypergraph.number_of_nodes());
    }

    // .ldb is a type of file used by Alchemy. This maps each node to its string representation for the ldb file.
    this->get_node_to_ldb_string_map();
}

ConceptPrinter::~ConceptPrinter() = default;

void ConceptPrinter::write_ldb_file(string filename) {
    filename += ".ldb";
    ofstream file;
    file.open(filename);
    if(file.is_open()){
        this->write_header(file);
        HyperGraphId temp_hypergraph_id{0};
        ConceptId temp_concept_id{0};

        for(auto concepts: this->concepts_vector){
            this->hypergraph_id = temp_hypergraph_id;
            for (auto& abstract_concept:get_values(concepts.get_concepts())) {
                this->concept_id = temp_concept_id;
                string string_type = "ldb";
                set<Atom> ldb_atoms = get_atoms_of_concept(abstract_concept, concepts.get_hypergraph(), string_type);
                this->write_atoms_to_file(ldb_atoms, file);
                temp_concept_id++;
            }
            temp_hypergraph_id++;
        }
        this->write_footer(file);
    }else{
        throw FileNotOpenedException(filename);
    }
}

void ConceptPrinter::write_uldb_file(string filename) {
    filename += ".uldb";
    ofstream file;
    file.open(filename);
    if(file.is_open()){
        this->write_header(file);
        HyperGraphId temp_hypergraph_id{0};
        ConceptId temp_concept_id{0};

        for(auto concepts: this->concepts_vector){
            this->hypergraph_id = temp_hypergraph_id;
            for (auto& abstract_concept:get_values(concepts.get_concepts())) {
                this->concept_id = temp_concept_id;
                string string_type = "uldb";
                set<Atom> uldb_atoms = get_atoms_of_concept(abstract_concept, concepts.get_hypergraph(), string_type);
                this->write_atoms_to_file(uldb_atoms, file);
                temp_concept_id++;
            }
            temp_hypergraph_id++;
        }
        this->write_footer(file);
    }else{
        throw FileNotOpenedException(filename);
    }

}

void ConceptPrinter::write_srcnclust_file(string filename) {
    filename += ".srcnclusts";
    ofstream file;
    file.open(filename);
    if(file.is_open()){
        this->write_header(file);
        HyperGraphId temp_hypergraph_id{0};
        for(auto concepts: this->concepts_vector){
            this->hypergraph_id = temp_hypergraph_id;
            ConceptId temp_concept_id{0};
            for (auto& abstract_concept:get_values(concepts.get_concepts())) {
                this->concept_id = temp_concept_id;
                NodePartition node_partition = get_node_ids(abstract_concept);
                this->write_concept_source_node_to_file(abstract_concept, file);
                this->write_single_node_ids_to_file(node_partition.single_nodes, file);
                this->write_cluster_node_ids_to_file(node_partition.clusters, file);
                this->write_all_node_ids_to_file(node_partition.single_nodes, node_partition.clusters, file);
                temp_concept_id++;
            }
            temp_hypergraph_id++;
        }
        this->write_footer(file);
    }else{
        throw FileNotOpenedException(filename);
    }

}

void ConceptPrinter::write_header(ofstream &file) {
    file <<"#START_GRAPH  #COMS "<<this->number_of_concepts<<endl<<endl;
}

void ConceptPrinter::write_atoms_to_file(set<string> &atoms, ofstream &file) {
    file << "#START_DB  " << this->concept_id << "  #COMS  1  #NUM_ATOMS " << atoms.size() << endl;
    for(auto &atom: atoms){
        file << atom << endl;
    }
    file << "#END_DB" << endl << endl;
}

void ConceptPrinter::write_footer(ofstream &file) {
    file<<"#END_GRAPH"<<endl;
}

void ConceptPrinter::write_concept_source_node_to_file(Concept &abstract_concept, ofstream &file) {
    file << "#START_DB  " << this->concept_id;
    file << "  #NUM_SINGLES " << abstract_concept.number_of_single_nodes ;
    file << "  #NUM_CLUSTS " << abstract_concept.number_of_clusters ;
    file << "  #NUM_NODES " << abstract_concept.number_of_nodes << endl;
    file << "SRC " << abstract_concept.source_node << endl;
}

void ConceptPrinter::write_single_node_ids_to_file(set<NodeId> &node_ids, ofstream &file) {
    for(auto node_id: node_ids){
        file << node_id << endl;
    }
}

void ConceptPrinter::write_cluster_node_ids_to_file(vector<Cluster> &cluster_node_ids, ofstream &file) {
    size_t cluster_id{0};
    for(auto node_ids: cluster_node_ids){
        stringstream string_of_node_ids;
        copy(node_ids.begin(), node_ids.end(), std::ostream_iterator<NodeId>(string_of_node_ids, " "));
        file << "CLUST " << cluster_id << "  " << string_of_node_ids.str() << endl;
        cluster_id++;
    }

}

void ConceptPrinter::write_all_node_ids_to_file(set<NodeId> &single_node_ids, vector<Cluster> &cluster_node_ids, ofstream &file) {
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

set<string> ConceptPrinter::get_atoms_of_concept(Concept &abstract_concept,
                                                 HyperGraph &hypergraph_of_concept,
                                                 string &string_type) {
    set<Atom> atoms;

    for(auto single_node: abstract_concept.single_nodes){
        atoms.merge(this->get_atoms_of_node_in_concept(single_node,
                                                       abstract_concept,
                                                       hypergraph_of_concept,
                                                       string_type));
    }
    for(auto &cluster:abstract_concept.clusters){
        for(auto cluster_node: cluster){
            atoms.merge(this->get_atoms_of_node_in_concept(cluster_node,
                                                           abstract_concept,
                                                           hypergraph_of_concept,
                                                           string_type));
        }
    }
    return atoms;
}

set<Atom> ConceptPrinter::get_atoms_of_node_in_concept(NodeId node,
                                                       Concept &abstract_concept,
                                                       HyperGraph &hypergraph_of_concept,
                                                       string &string_type) {
    set<Atom> atoms;
    vector<EdgeId> all_edges = hypergraph_of_concept.get_memberships(node);
    for(auto edge: all_edges){
        vector<NodeId> nodes_of_edge = hypergraph_of_concept.get_nodes_of_edge(edge);
        set<NodeId> nodes_of_edge_set(nodes_of_edge.begin(), nodes_of_edge.end());
        if(includes(abstract_concept.nodes.begin(), abstract_concept.nodes.end(), nodes_of_edge_set.begin(), nodes_of_edge_set.end())){
            string predicate = hypergraph_of_concept.get_predicate(edge).data();
            atoms.insert(this->get_atom_for_edge(predicate, nodes_of_edge, string_type));
        }else{
            continue;
        }
    }
    return atoms;
}

Atom ConceptPrinter::get_atom_for_edge(const Predicate &edge_predicate, vector<NodeId> &nodes_of_edge, string &string_type) {
    Atom atom = edge_predicate + "(";
    vector<NodeName> node_names = get_node_names(nodes_of_edge, string_type);
    atom += boost::algorithm::join(node_names, ",") + ")";
    return atom;
}

vector<NodeName> ConceptPrinter::get_node_names(vector<size_t> &nodes_of_edge, string &string_type) {
    vector<NodeName> node_names;
    if(string_type == "ldb"){
        for(auto node:nodes_of_edge){
            node_names.emplace_back(this->node_to_ldb_string[this->hypergraph_id][this->concept_id][node]);
        }
        return node_names;
    }else {
        for(auto node:nodes_of_edge){
            node_names.emplace_back("NODE_"+to_string(node));
        }
        return node_names;
    }
}

NodePartition ConceptPrinter::get_node_ids(Concept &abstract_concept) {
    NodePartition node_partition;
    node_partition.single_nodes = abstract_concept.single_nodes;
    node_partition.clusters = abstract_concept.clusters;
    return node_partition;
}

void ConceptPrinter::get_node_to_ldb_string_map() {
    HyperGraphId temp_hypergraph_id{0};
    ConceptId temp_concept_id{0};

    for(auto concepts:this->concepts_vector){
        for(const auto &abstract_concept: get_values(concepts.get_concepts())){
            for(auto single_node: abstract_concept.single_nodes){
                this->node_to_ldb_string[temp_hypergraph_id][temp_concept_id][single_node] = "NODE_" + to_string(single_node);
            }
            size_t cluster_id{0};
            for(const auto &cluster: abstract_concept.clusters){
                for(auto cluster_node : cluster){
                    this->node_to_ldb_string[temp_hypergraph_id][temp_concept_id][cluster_node] = "CLUST_" + to_string(cluster_id);
                }
                cluster_id++;
            }
            temp_concept_id++;
        }
        temp_hypergraph_id++;
    }
}

void ConceptPrinter::write_files(string &filename) {
    this->write_ldb_file(filename);
    this->write_uldb_file(filename);
    this->write_srcnclust_file(filename);

}
