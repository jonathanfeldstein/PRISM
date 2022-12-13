#include "Concepts.h"


Concepts::Concepts(HyperGraph hypergraph, RandomWalkerConfig config)
        : random_walker(hypergraph, config) {
    config.check_config();
    Timer timer("Concepts");
    this->hypergraph = hypergraph;

    if(hypergraph.get_estimated_graph_diameter() == 0){
        cout<<"Warning: No hierarchical clustering was performed."<<endl;
        hypergraph.compute_diameter();
    }

    if(config.multiprocessing){
        omp_lock_t concept_support_lock;
        omp_init_lock(&concept_support_lock);
        set<NodeId> node_ids_set = this->hypergraph.get_node_ids();
        vector<NodeId> node_ids(node_ids_set.begin(), node_ids_set.end());
        #pragma omp parallel for schedule(dynamic)
        for(int i=0; i< node_ids.size(); i++){
            if(this->hypergraph.check_is_source_node(node_ids[i])){
                Concept abstract_concept = this->get_concept(node_ids[i], config);
                omp_set_lock(&concept_support_lock);
                this->concepts.insert({node_ids[i], abstract_concept});
                omp_unset_lock(&concept_support_lock);

            }
        }
    }else{
        for(auto node: this->hypergraph.get_node_ids()){
            if(this->hypergraph.check_is_source_node(node)){
                this->concepts.insert({node, this->get_concept(node, config)});
            }
        }
    }
}

size_t Concepts::size() {
    return this->concepts.size();
}

HyperGraph &Concepts::get_hypergraph() {
    return this->hypergraph;
}


void Concepts::print() {
    string output_str;
    size_t concept_id{1};
    for(Concept abstract_concept:get_values(this->concepts)){
        output_str += "COMMUNITY " + to_string(concept_id) + " \n" + abstract_concept.print();
        concept_id++;
    }
    cout<< output_str;
}

map<NodeId, Concept> Concepts::get_concepts() {
    return this->concepts;
}

Concept Concepts::get_concept(NodeId source_node, RandomWalkerConfig config) {
    map<NodeId , NodeRandomWalkData> random_walk_data = this->random_walker.generate_node_random_walk_data(source_node);
    random_walk_data.erase(source_node);
    set<NodeId> single_nodes = {source_node};
    vector<Cluster> clusters;
    double theta_sym = compute_theta_sym(config.alpha,
            this->random_walker.get_number_of_walks_ran(),
            this->random_walker.get_length_of_walk());
    set<NodeRandomWalkData> close_nodes = get_commonly_encountered_nodes(random_walk_data,
            this->random_walker.get_number_of_walks_ran(), config.epsilon);

    for(auto const& node_type: this->hypergraph.get_node_types()){
        RandomWalkCluster nodes_of_type;
        for(auto node: close_nodes){
            if(node.get_node_type() == node_type){
                nodes_of_type.push_back(node);
            }
        }
        if(!nodes_of_type.empty()){
            NodePartition node_partition;
            if(nodes_of_type.size() ==1){
                node_partition.single_nodes.insert(nodes_of_type[0].get_node_id());
            }else{
                node_partition =cluster_nodes_by_path_similarity(nodes_of_type,
                                                                               this->random_walker.get_number_of_walks_ran(),
                                                                               this->random_walker.get_length_of_walk(),
                                                                               theta_sym,
                                                                               config);
//                node_partition =cluster_nodes_by_birch(nodes_of_type,
//                                                       config.pca_dim,
//                                                       config.num_top_paths_for_clustering,
//                                                       this->random_walker.get_number_of_walks_ran(),
//                                                       config.alpha);
//                node_partition = cluster_nodes_by_sk_divergence(nodes_of_type,config.alpha,
//                                                                this->random_walker.get_number_of_walks_ran(),
//                                                                config.num_top_paths_for_clustering);
            }

            single_nodes.merge(node_partition.single_nodes);
            clusters.insert(clusters.end(), node_partition.clusters.begin(), node_partition.clusters.end());
        }
    }
    return Concept(source_node, single_nodes, clusters);
}


