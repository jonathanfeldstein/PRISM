//
// Created by jonathan on 21/03/2022.
//

#include "Communities.h"


Communities::Communities(HyperGraph hypergraph, RandomWalkerConfig config)
        : random_walker(hypergraph, config) {
    Timer timer("Communities");
    this->hypergraph = hypergraph;

    if(hypergraph.get_estimated_graph_diameter() == 0){ // TODO this is a problem it should not always compute -> need another function
        cout<<"Warning: UndirectedGraph diameter of the hypergraph not known."
              " Reverting to using default length of random walks."<<endl;
    }

    if(config.multiprocessing){
        // TODO if multiprocessing enabled
        omp_lock_t community_support_lock;
        omp_init_lock(&community_support_lock);
        set<size_t> node_ids_set = this->hypergraph.get_node_ids();
        vector<size_t> node_ids(node_ids_set.begin(), node_ids_set.end());
        #pragma omp parallel for schedule(dynamic)
        for(int i=0; i< node_ids.size(); i++){ //TODO Check whether possible const&
            if(this->hypergraph.check_is_source_node(node_ids[i])){
                Community community = this->get_community(node_ids[i], config);
                omp_set_lock(&community_support_lock);
                this->communities.insert({node_ids[i], community});
                omp_unset_lock(&community_support_lock);

            }
        }
    }else{
        for(auto node: this->hypergraph.get_node_ids()){ //TODO Check whether possible const&
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

void Communities::check_arguments() {
    //TODO
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

map<size_t, Community> Communities::get_communities() {
    return this->communities;
}

Community Communities::get_community(size_t &source_node, RandomWalkerConfig config) {
    map<size_t , NodeRandomWalkData> random_walk_data = this->random_walker.generate_node_random_walk_data(source_node);
    random_walk_data.erase(source_node);
    set<size_t> single_nodes = {source_node}; //TODO CHeck if that works, as it is a reference
    vector<set<size_t>> clusters;
    double theta_sym = compute_theta_sym(config.alpha,
            this->random_walker.get_number_of_walks_ran(),
            this->random_walker.get_length_of_walk());
    set<NodeRandomWalkData> close_nodes = get_commonly_encountered_nodes(random_walk_data,
            this->random_walker.get_number_of_walks_ran(), config.epsilon);

    for(string const& node_type: this->hypergraph.get_node_types()){
        vector<NodeRandomWalkData> nodes_of_type;
        for(auto node: close_nodes){
            if(node.get_node_type() == node_type){
                nodes_of_type.push_back(node);
            }
        }
        if(!nodes_of_type.empty()){
            pair<set<size_t>, vector<set<size_t>>> single_nodes_and_clusters_of_type =
                                                    cluster_nodes_by_path_similarity(nodes_of_type,
                                                    this->random_walker.get_number_of_walks_ran(),
                                                    this->random_walker.get_length_of_walk(),
                                                    theta_sym,
                                                    config);
            single_nodes.merge(single_nodes_and_clusters_of_type.first);
            clusters.insert(clusters.end(), single_nodes_and_clusters_of_type.second.begin(), single_nodes_and_clusters_of_type.second.end());
        }
    }
    return Community(source_node, single_nodes, clusters);
}


