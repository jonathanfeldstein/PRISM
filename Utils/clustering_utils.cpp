//
// Created by jonathan on 14/04/2022.
//

#include "clustering_utils.h"
#include "hypothesis_testing.h"

double compute_theta_sym(double alpha, size_t number_of_walks_ran, size_t length_of_walk){
    math::students_t t_distr(number_of_walks_ran-1);
    double t_statistic_critical_value = quantile(complement(t_distr, alpha/2));

    return (((double)length_of_walk -1)/pow(2*number_of_walks_ran, 0.5))*t_statistic_critical_value;
}

set<NodeRandomWalkData> get_commonly_encountered_nodes(const map<NodeId, NodeRandomWalkData> &nodes_random_walk_data, size_t number_of_walks_ran, double epsilon){
    set<NodeRandomWalkData> commonly_encountered_nodes;
    for(auto node : nodes_random_walk_data){
        if(node.second.get_count_of_nth_path(3) >= number_of_walks_ran/(number_of_walks_ran*pow(epsilon, 2)+1)){
            commonly_encountered_nodes.insert(node.second);
        }
    }
    return commonly_encountered_nodes;
}

NodePartition cluster_nodes_by_path_similarity(vector<NodeRandomWalkData> nodes_of_type,
                                               size_t number_of_walks_ran,
                                               size_t length_of_walks,
                                               double theta_sym,
                                               RandomWalkerConfig &config){
    /*
     *  Clusters nodes from a hypergraph into groups which are symmetrically related relative to a source node.
     *  Firstly, nodes are grouped into distance-symmetric clusters; sets of nodes where the difference in the average
     *  truncated hitting times from the source node for any two nodes in the set is no greater than a specified threshold.
     *
     *  Secondly, within each distance-symmetric cluster, we further cluster together nodes based on the similarity
     *  in their distribution of paths to yield a set of path-symmetric clusters. If the cluster is a singleton set then
     *  we add its member to a set of 'single nodes'.
     *
     *  returns: the set of single nodes and a list of path-symmetric node clusters
     */

    set<size_t> single_nodes;
    vector<set<size_t>> clusters;
    NodePartition path_similarity_partition;

    pair<set<size_t>, vector<vector<NodeRandomWalkData>>> distance_symmetric_clusters = cluster_nodes_by_truncated_hitting_times(nodes_of_type, theta_sym);

    path_similarity_partition.single_nodes.merge(distance_symmetric_clusters.first);

    for (auto &distance_symmetric_cluster: distance_symmetric_clusters.second) {
        NodePartition path_symmetric_partition = cluster_nodes_by_path_distribution(distance_symmetric_cluster,
                                                                                    number_of_walks_ran,
                                                                                    length_of_walks,
                                                                                    config);

        path_similarity_partition.single_nodes.merge(path_symmetric_partition.single_nodes);
        path_similarity_partition.clusters.insert(clusters.end(), path_symmetric_partition.clusters.begin(), path_symmetric_partition.clusters.end());

    }

    return path_similarity_partition;
}


pair<set<NodeId>, vector<vector<NodeRandomWalkData>>> cluster_nodes_by_truncated_hitting_times(vector<NodeRandomWalkData> nodes_of_type,
                                                                                               double threshold_hitting_time_difference) {

    // TODO check that this indeed sorts from least to biggest
    sort(nodes_of_type.begin(), nodes_of_type.end());
    double current_hitting_time = nodes_of_type[0].get_average_hitting_time();
    vector<vector<NodeRandomWalkData>> distance_symmetric_clusters;
    vector<NodeRandomWalkData> distance_symmetric_cluster;
    set<NodeId> distance_symmetric_single_nodes;

    for(auto node: nodes_of_type){
        if(node.get_average_hitting_time()-current_hitting_time < threshold_hitting_time_difference){
            distance_symmetric_cluster.emplace_back(node);
        }else{
            if(distance_symmetric_cluster.size() == 1){
                distance_symmetric_single_nodes.insert(distance_symmetric_cluster[0].get_node_id());
            }else{
                distance_symmetric_clusters.emplace_back(distance_symmetric_cluster);
            }
            distance_symmetric_cluster.clear();
            distance_symmetric_cluster.emplace_back(node);
        }
        current_hitting_time = node.get_average_hitting_time();
    }

    if (!distance_symmetric_cluster.empty()) {
        if (distance_symmetric_cluster.size() > 1) {
            distance_symmetric_clusters.emplace_back(distance_symmetric_cluster);
        } else {
            distance_symmetric_single_nodes.insert(distance_symmetric_cluster[0].get_node_id());
        }
    }

    return {distance_symmetric_single_nodes, distance_symmetric_clusters};
}

NodePartition cluster_nodes_by_path_distribution(const vector<NodeRandomWalkData> &nodes_of_type,
                                                                          size_t number_of_walks,
                                                                          size_t length_of_walks,
                                                                          RandomWalkerConfig &config){
    NodePartition path_distribution_partition;

    if (hypothesis_test_path_symmetric_nodes(nodes_of_type, number_of_walks, config.num_top_paths_for_clustering, length_of_walks, config.alpha)) {
        Cluster cluster;
        for(auto &node:nodes_of_type){
            cluster.insert(node.get_node_id());
        }
        path_distribution_partition.clusters.emplace_back(cluster);
    }else{
        cout << "Hypothesis test has finally failed!" << endl;
        if(nodes_of_type.size() <= config.clustering_method_threshold){
            cout << "SK div rules!!!" << endl;
            NodePartition sk_partition = cluster_nodes_by_sk_divergence(nodes_of_type,
                                                                        config.alpha,
                                                                        number_of_walks,
                                                                        config.num_top_paths_for_clustering);
            path_distribution_partition.single_nodes = sk_partition.single_nodes; // TODO Jonathan Check for potential memory leakage
            path_distribution_partition.clusters = sk_partition.clusters;
        }else{
            cout << "BIRCH rules (yeah it really it BIRCH screw you haters)!!" << endl;
            if(nodes_of_type.size() ==1){
                path_distribution_partition.single_nodes.insert(nodes_of_type[0].get_node_id());
            }else{
                NodePartition birch_partition = cluster_nodes_by_birch(nodes_of_type,
                                                                       config.pca_dim,
                                                                       config.num_top_paths_for_clustering,
                                                                       number_of_walks,
                                                                       config.alpha);
                path_distribution_partition.single_nodes = birch_partition.single_nodes;
                path_distribution_partition.clusters = birch_partition.clusters;
            }
        }
    }
    return path_distribution_partition;
}

MatrixXd compute_top_paths(const vector<NodeRandomWalkData> &nodes_of_type, size_t max_number_of_paths, size_t path_length) {
    vector<vector<pair<Path, int>>> top_paths_of_each_node;
    for (auto node: nodes_of_type) {
        top_paths_of_each_node.emplace_back(node.get_top_paths(max_number_of_paths, path_length));
    }
    set<string> unique_paths;
    for (const auto& paths: top_paths_of_each_node) {
        for (const auto& path_pair: paths) {
            unique_paths.insert(path_pair.first);
        }
    }

    size_t number_of_unique_paths;
    number_of_unique_paths = unique_paths.size();
    unique_paths.clear();

    MatrixXd node_path_counts = MatrixXd::Zero(nodes_of_type.size(), number_of_unique_paths);
    if (number_of_unique_paths > 0) {
        map<Path, size_t> path_string_to_path_index;
        size_t node_index{0};
        for (const auto& node_paths: top_paths_of_each_node) {
            for (const auto& path: node_paths) {
                size_t path_index{};
                if (!has(get_keys(path_string_to_path_index), path.first)) {
                    path_index = path_string_to_path_index.size();
                    path_string_to_path_index[path.first] = path_index;
                } else {
                    path_index = path_string_to_path_index[path.first];
                }
                node_path_counts(node_index, path_index) = path.second;
            }
            node_index++;
        }
    }

    return node_path_counts;

}

NodePartition cluster_nodes_by_sk_divergence(const vector<NodeRandomWalkData> &nodes_of_type,
                                                                      double significance_level,
                                                                      size_t number_of_walks,
                                                                      size_t max_number_of_paths) {
    vector<NodeClusterRandomWalkData> sk_clusters;
    for (auto node: nodes_of_type) {
        sk_clusters.emplace_back(NodeClusterRandomWalkData(node));
    }

    double max_divergence = numeric_limits<double>::infinity();

    size_t cluster_to_merge1{0};
    size_t cluster_to_merge2{0};
    while (true) {
        double smallest_divergence = max_divergence;
        for (size_t i = 0; i < sk_clusters.size() ; i++) {
            for (size_t j = i + 1; j < sk_clusters.size(); j++) {
                pair<double, double> sk_divergence_and_threshold = compute_sk_divergence_of_top_n_paths(sk_clusters[i], sk_clusters[j], max_number_of_paths, number_of_walks, significance_level);

                if (sk_divergence_and_threshold.first < smallest_divergence && sk_divergence_and_threshold.first < sk_divergence_and_threshold.second) {
                    smallest_divergence = sk_divergence_and_threshold.first;
                    cluster_to_merge1 = i;
                    cluster_to_merge2 = j;
                }
            }
        }

        if (smallest_divergence < max_divergence) {
            sk_clusters[cluster_to_merge1].merge(sk_clusters[cluster_to_merge2]);
            sk_clusters.erase(sk_clusters.begin()+cluster_to_merge2);
        } else {
            break;
        }
    }

    NodePartition sk_partition;
    for (auto sk_cluster: sk_clusters) {
        if (sk_cluster.number_of_nodes() == 1) {
            size_t node_id = *sk_cluster.get_node_ids().begin();
            sk_partition.single_nodes.insert(node_id);
        } else {
            sk_partition.clusters.emplace_back(sk_cluster.get_node_ids());
        }
    }

    return sk_partition;
}


NodePartition cluster_nodes_by_birch(const vector<NodeRandomWalkData> &nodes,
                                                              int pca_target_dimension,
                                                              int max_number_of_paths,
                                                              int number_of_walks,
                                                              double significance_level) {
    /*
     * Considers the top number_of_paths most frequent paths for each node, standardises these path distributions, then
     * dimensionality reduces them using PCA (from a space of dimension equal to the number of distinct paths into a space
     * of dimension equal to pca_target_dimension), then finally performs birch clustering on these principal components,
     * where the optimal number of clusters is the smallest value whose clusters all pass the hypothesis test of the
     * nodes having path distributions consistent with being statistically similar.
     *
     * :param nodes: The nodes to be clustered.
     * :param pca_target_dimension: The dimension of feature space after dimensionality reduction with PCA.
     * :param max_number_of_paths: The number of paths to consider for the path distribution feature vectors (we consider
     *                              the number_of_paths most common).
     * :param number_of_walks: the number of random walks that were run on the cluster to generate the random walk data
     * :param significance_level: the desired significance level for the hypothesis test of nodes being path symmetric.
     * Smaller values means that a larger difference between the distribution of the node's
     * paths is tolerated before they are considered not path-symmetric.
     * :return: single_nodes, clusters: the final clustering of the nodes
     * */

    MatrixXd node_path_counts = compute_top_paths(nodes, max_number_of_paths, 0);
    MatrixXd feature_vectors = compute_principal_components(node_path_counts,
                                                            pca_target_dimension);

    // TODO fix number of iterations and threshold
    vector<size_t> clustering_labels = hierarchical_two_means(node_path_counts,
                                                              feature_vectors,
                                                              100, //Iterations and convergence threshold changes do not impact the 2-means algorithm as it directly finds optimal centroids
                                                              0.01,
                                                              number_of_walks,
                                                              significance_level);


    return group_nodes_by_clustering_labels(nodes, clustering_labels);
}



MatrixXd compute_principal_components(MatrixXd &feature_vectors, int target_dimension){
    /*
     * Dimensionality reduces feature vectors into a target dimension using Principal Component Analysis.
     * :param feature_vectors: (dimension_of_feature_vectors) x (number_of_feature_vectors)
     * :param target_dimension: the desired dimension of the dimensionality-reduced data
     * :return: principal_components: the dimensionality-reduced feature vectors
     */

    size_t original_dimension = feature_vectors.cols();
    if (original_dimension > target_dimension){
        return PCA(feature_vectors, target_dimension);
    }
    return feature_vectors;
}


MatrixXd get_node_path_counts_of_cluster(MatrixXd &node_path_counts,
                                         vector<size_t> &cluster_labels,
                                         size_t label_to_select){
    // Gets the subset of node path counts corresponding to those nodes that have a specified clustering label

    vector<int> indices_of_nodes_in_cluster = find_indices_of_element(cluster_labels, label_to_select);

    return node_path_counts(indices_of_nodes_in_cluster,all);
}

size_t two_means(vector<size_t> &cluster_labels,
                 MatrixXd &all_points,
                 int max_iterations,
                 double threshold,
                 size_t cluster_label_to_split){ //Each column is a point
    size_t new_cluster_label = *max_element(cluster_labels.begin(), cluster_labels.end())+1;
    size_t total_points = all_points.rows();

    // Initializing Clusters
    vector<int> indices_of_labels_to_split = find_indices_of_element(cluster_labels, (int)cluster_label_to_split);
    VectorXd centroid1 = all_points.row(indices_of_labels_to_split[0]);
    VectorXd centroid2 = all_points.row(indices_of_labels_to_split[1]);
    int iter{0};
    while(true){
        MatrixXd cluster1 = Eigen::MatrixXd::Zero(all_points.rows(), all_points.cols());
        MatrixXd cluster2 = Eigen::MatrixXd::Zero(all_points.rows(), all_points.cols());
        bool converged = false;
        int nodes_in_cluster1 = 0;
        int nodes_in_cluster2 = 0;
        for(int i{0}; i<total_points; i++){
            if(cluster_labels[i] != cluster_label_to_split && cluster_labels[i] != new_cluster_label && cluster_labels[i] != new_cluster_label+1){
                continue;
            }
            //assign i to nearest cluster;
            VectorXd displacement_to_1 = centroid1.transpose() - all_points.row(i);
            VectorXd displacement_to_2 = centroid2.transpose() - all_points.row(i);
            double distance1 = displacement_to_1.dot(displacement_to_1);
            double distance2 = displacement_to_2.dot(displacement_to_2);
            if(distance1 <distance2){
                cluster_labels[i] = new_cluster_label;
                cluster1.row(i) = all_points.row(i);
                nodes_in_cluster1++;
            }else{
                cluster_labels[i] = new_cluster_label+1;
                cluster2.row(i) = all_points.row(i);
                nodes_in_cluster2++;
            }

        }
        //compute new centroid
        VectorXd new_centroid1;
        new_centroid1 = cluster1.colwise().sum() * (1/(double)nodes_in_cluster1);
        VectorXd new_centroid2;
        new_centroid2 = cluster2.colwise().sum() * (1/(double)nodes_in_cluster2);

        double distance = pow((new_centroid1 - centroid1).dot(new_centroid1 -centroid1) + (new_centroid2 - centroid2).dot(new_centroid2 -centroid2),0.5);
        //determine convergence
        if(distance < threshold){
            converged = true;
        }
        if (converged || iter >= max_iterations)
        {
            break;
        }
        centroid1 = new_centroid1;
        centroid2 = new_centroid2;
        iter++;
    }
    return new_cluster_label;

}

NodePartition group_nodes_by_clustering_labels(const vector<NodeRandomWalkData> &nodes,
                                               vector<size_t> cluster_labels) {
    /*
     * Groups a list of nodes into single nodes and clusters from a list of cluster labels.
     * :param nodes: the nodes to be grouped
     * :param cluster_labels: a list of integers assigning each node to a given cluster
     */

    size_t number_of_clusters = set<size_t> (cluster_labels.begin(), cluster_labels.end()).size();
    vector<Cluster> original_clusters(number_of_clusters);
    for (int node_index = 0; node_index < cluster_labels.size(); node_index++) {
        size_t cluster_index = cluster_labels[node_index];
        original_clusters[cluster_index].insert(nodes[node_index].get_node_id());
    }

    // split into single nodes and clusters
    NodePartition birch_partition;
    for (auto &cluster: original_clusters) {
        if (cluster.size() == 1) {
            size_t node_id = *(cluster.begin());
            birch_partition.single_nodes.insert(node_id);
        }
        else {
            birch_partition.clusters.emplace_back(cluster);
        }
    }

    return birch_partition;
}

vector<size_t> hierarchical_two_means(MatrixXd node_path_counts,
                                      MatrixXd &node_feature_vectors,
                                      int max_iterations,
                                      double convergence_threshold,
                                      int number_of_walks,
                                      double theta_p){

    vector<size_t> cluster_labels(node_feature_vectors.rows(), 0);

    set<size_t> failed_labels;
    set<size_t> good_cluster_labels;
    set<size_t> new_labels;

    // Check if all points collectively pass a hypothesis test of being path symmetric
    bool passed = hypothesis_test_on_node_path_counts(node_path_counts, number_of_walks, theta_p);
    if(passed){
        return cluster_labels;
    } else {
        failed_labels.insert(0);
    }

    // Recursively check if there are any failed labels remaining, and if so call 2-means clustering on those points
    while(!failed_labels.empty()){
        for(auto failed_label:failed_labels){
            size_t new_label = two_means(cluster_labels,
                                         node_feature_vectors,
                                         max_iterations,
                                         convergence_threshold,
                                         failed_label);    // internally updates cluster_labels
            new_labels.insert(new_label);
            new_labels.insert(new_label+1);
            for (auto label: cluster_labels) {
                cout << label << " ";
            }
            cout << endl;
        }
        failed_labels.clear();
        for(auto label:new_labels){
            MatrixXd node_path_counts_of_nodes_in_cluster = get_node_path_counts_of_cluster(node_path_counts, cluster_labels, label);
            if (node_path_counts_of_nodes_in_cluster.rows() > 1) {
                passed = hypothesis_test_on_node_path_counts(node_path_counts_of_nodes_in_cluster, number_of_walks, theta_p);
            }else{
                passed = true;
            }
            if(passed){
                good_cluster_labels.insert(label);
            }else{
                failed_labels.insert(label);
            }
        }
        new_labels.clear();
    }

    cluster_labels = standardize_cluster_labels(cluster_labels);

    return cluster_labels;
}


vector<size_t> standardize_cluster_labels(vector<size_t> cluster_labels) {

    vector<size_t> standardized_cluster_labels;
    map<size_t, size_t> relabelling_map;
    set<size_t> labels_seen_so_far;

    for (size_t label: cluster_labels) {
        if (!has(labels_seen_so_far,label)) {
            relabelling_map[label] = labels_seen_so_far.size();
            labels_seen_so_far.insert(label);
        }
        standardized_cluster_labels.emplace_back(relabelling_map[label]);
    }

    return standardized_cluster_labels;
}