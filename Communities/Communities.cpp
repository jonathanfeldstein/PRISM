//
// Created by jonathan on 21/03/2022.
//

#include "Communities.h"


Communities::Communities(HyperGraph hypergraph, RandomWalkerConfig config)
        : random_walker(hypergraph, config) {
    config.check_config();
    Timer timer("Communities");
    this->hypergraph = hypergraph;

    if(hypergraph.get_estimated_graph_diameter() == 0){ // TODO is the comment below actually true?
        cout<<"Warning: UndirectedGraph diameter of the hypergraph not known."
              " Reverting to using default length of random walks."<<endl;
    }

    if(config.multiprocessing){ // TODO Remove Non MultiProcessing
        omp_lock_t community_support_lock;
        omp_init_lock(&community_support_lock);
        set<NodeId> node_ids_set = this->hypergraph.get_node_ids();
        vector<NodeId> node_ids(node_ids_set.begin(), node_ids_set.end());
        #pragma omp parallel for schedule(dynamic)
        for(int i=0; i< node_ids.size(); i++){
            if(this->hypergraph.check_is_source_node(node_ids[i])){
                Community community = this->get_community(node_ids[i], config);
                omp_set_lock(&community_support_lock);
                this->communities.insert({node_ids[i], community});
                omp_unset_lock(&community_support_lock);

            }
        }
    }else{
        for(auto node: this->hypergraph.get_node_ids()){
            if(this->hypergraph.check_is_source_node(node)){
                this->communities.insert({node, this->get_community(node, config)});
            }
        }
    }
}

Communities::~Communities() {

}

size_t Communities::size() {
    return this->communities.size();
}

HyperGraph &Communities::get_hypergraph() {
    return this->hypergraph;
}


void Communities::print() {
    string output_str;
    size_t community_id{1};
    for(Community community:get_values(this->communities)){
        output_str += "COMMUNITY " + to_string(community_id) + " \n" + community.print();
        community_id++;
    }
    cout<< output_str;
}

map<NodeId, Community> Communities::get_communities() {
    return this->communities;
}

Community Communities::get_community(NodeId &source_node, RandomWalkerConfig config) {
    map<NodeId , NodeRandomWalkData> random_walk_data = this->random_walker.generate_node_random_walk_data(source_node);
    random_walk_data.erase(source_node);
    set<NodeId> single_nodes = {source_node}; //TODO CHeck if that works, as it is a reference
    vector<Cluster> clusters;
    double theta_sym = compute_theta_sym(config.alpha,
            this->random_walker.get_number_of_walks_ran(),
            this->random_walker.get_length_of_walk());
    set<NodeRandomWalkData> close_nodes = get_commonly_encountered_nodes(random_walk_data,
            this->random_walker.get_number_of_walks_ran(), config.epsilon);

    for(auto const& node_type: this->hypergraph.get_node_types()){
        vector<NodeRandomWalkData> nodes_of_type;
        for(auto node: close_nodes){
            if(node.get_node_type() == node_type){
                nodes_of_type.push_back(node);
            }
        }
        if(!nodes_of_type.empty()){
            pair<set<NodeId>, vector<Cluster>> single_nodes_and_clusters_of_type =
                                                    cluster_nodes_by_path_similarity(nodes_of_type,
                                                    this->random_walker.get_number_of_walks_ran(),
                                                    this->random_walker.get_length_of_walk(),
                                                    theta_sym,
                                                    config); // TODO introduce type for pair<single_nodes, vector<Cluster>>
            single_nodes.merge(single_nodes_and_clusters_of_type.first);
            clusters.insert(clusters.end(), single_nodes_and_clusters_of_type.second.begin(), single_nodes_and_clusters_of_type.second.end());
        }
    }
    return Community(source_node, single_nodes, clusters);
}


