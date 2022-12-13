#ifndef PRISM_CONCEPTPRINTER_H
#define PRISM_CONCEPTPRINTER_H

#include <map>
#include <string>
#include "Concepts.h"
#include <boost/algorithm/string/join.hpp>
#include "../Utils/vector_utils.h"
#include "exceptions.h"
#include "Relation.h"
#include "Partition.h"

using namespace std;
using NodeId = size_t;
using EdgeId = size_t;
using Predicate = string;
using HyperGraphId = size_t;
using ConceptId = size_t;
using Atom = string;
using NodeName = string;
using Cluster = set<NodeId>;

class ConceptPrinter{
private:
    vector<Concepts> concepts_vector;
    size_t number_of_concepts{0};
    map<HyperGraphId, map<ConceptId, map<NodeId, NodeName>>> node_to_ldb_string;
    HyperGraphId hypergraph_id{0};
    ConceptId concept_id{0};

    void write_ldb_file(string filename);
    void write_uldb_file(string filename);
    void write_srcnclust_file(string filename);
    void write_header(ofstream &file);
    void write_atoms_to_file(set<string> &atoms, ofstream &file);
    void write_footer(ofstream &file);
    void write_concept_source_node_to_file(Concept &abstract_concept, ofstream &file);
    void write_single_node_ids_to_file(set<NodeId> &node_ids, ofstream &file);
    void write_cluster_node_ids_to_file(vector<Cluster> &cluster_node_ids, ofstream &file);
    void write_all_node_ids_to_file(set<NodeId> &single_node_ids, vector<Cluster> &cluster_node_ids, ofstream &file);

    set<Atom> get_atoms_of_concept(Concept &abstract_concept,
                                   HyperGraph &hypergraph_of_concept,
                                   string &string_type);

    set<Atom> get_atoms_of_node_in_concept(NodeId node,
                                           Concept &abstract_concept,
                                           HyperGraph &hypergraph_of_concept,
                                           string &string_type);

    Atom get_atom_for_edge(const Predicate &edge_predicate,
                           vector<NodeId> &nodes_of_edge,
                           string &string_type);

    vector<NodeName> get_node_names(vector<NodeId> &nodes_of_edge,
                                    string &string_type);

    NodePartition get_node_ids(Concept &abstract_concept);
    void get_node_to_ldb_string_map();

public:
    ConceptPrinter(vector<Concepts> concepts_vector, HyperGraph original_hypergraph);
    ~ConceptPrinter();
    void write_files(string &filename);

};
#endif //PRISM_CONCEPTPRINTER_H
