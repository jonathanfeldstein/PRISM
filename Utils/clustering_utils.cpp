//
// Created by jonathan on 14/04/2022.
//

#include "clustering_utils.h"
#include "hypothesis_testing.h"

double compute_theta_sym(double theta_p, size_t number_of_walks_ran, size_t length_of_walk){
    math::students_t t_distr(number_of_walks_ran-1);
    double t_statistic_critical_value = quantile(complement(t_distr, theta_p/2));

    return (((double)length_of_walk -1)/pow(2*number_of_walks_ran, 0.5))*t_statistic_critical_value;
}

set<NodeRandomWalkData> get_commonly_encountered_nodes(const map<size_t, NodeRandomWalkData> &nodes_random_walk_data, size_t number_of_walks_ran, double epsilon){
    set<NodeRandomWalkData> commonly_encountered_nodes;
    for(auto node : nodes_random_walk_data){
        if(node.second.get_count_of_nth_path(3) >= number_of_walks_ran/(number_of_walks_ran*pow(epsilon, 2)+1)){
            commonly_encountered_nodes.insert(node.second);
        }
    }
    return commonly_encountered_nodes;
}

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_path_similarity(vector<NodeRandomWalkData> nodes_of_type,
                                                                        size_t number_of_walks_ran,
                                                                        size_t length_of_walks,
                                                                        double theta_sym,
                                                                        RandomWalkerConfig &config){
    //    Clusters nodes from a hypergraph into groups which are symmetrically related relative to a source node.
    //
    //            Firstly, nodes are grouped into distance-symmetric clusters; sets of nodes where the difference in the average
    //    truncated hitting times from the source node for any two nodes in the set is no greater than a specified threshold.
    //
    //            Secondly, within each distance-symmetric cluster, we further cluster together nodes based on the similarity
    //    in their distribution of paths to yield a set of path-symmetric clusters. If the cluster is a singleton set then we
    //    add its member to a set of 'single nodes'.
    //
    //            returns: the set of single nodes and a list of path-symmetric node clusters
    set<size_t> single_nodes;
    vector<set<size_t>> clusters;

    pair<set<size_t>, vector<vector<NodeRandomWalkData>>> distance_symmetric_clusters = cluster_nodes_by_truncated_hitting_times(
            nodes_of_type, theta_sym);

    single_nodes.merge(distance_symmetric_clusters.first);

    for (auto distance_symmetric_cluster: distance_symmetric_clusters.second) {
        pair<set<size_t>, vector<set<size_t>>> path_symmetric_clusters = cluster_nodes_by_path_distribution(distance_symmetric_cluster,
                                                                                                            number_of_walks_ran,
                                                                                                            length_of_walks,
                                                                                                            config);

        single_nodes.merge(path_symmetric_clusters.first);
        clusters.insert(clusters.end(), path_symmetric_clusters.second.begin(), path_symmetric_clusters.second.end());

    }

    return {single_nodes, clusters};
}

pair<set<size_t>, vector<vector<NodeRandomWalkData>>> cluster_nodes_by_truncated_hitting_times(vector<NodeRandomWalkData> nodes_of_type,
                                                                                               double threshold_hitting_time_difference) {

    // TODO check that this indeed sorts from least to biggest
    sort(nodes_of_type.begin(), nodes_of_type.end());
    double current_hitting_time = nodes_of_type[0].get_average_hitting_time();
    vector<vector<NodeRandomWalkData>> distance_symmetric_clusters;
    vector<NodeRandomWalkData> distance_symmetric_cluster;
    set<size_t> distance_symmetric_single_nodes;

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

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_path_distribution(const vector<NodeRandomWalkData> &nodes_of_type,
                                                                          size_t number_of_walks,
                                                                          size_t length_of_walks,
                                                                          RandomWalkerConfig &config){
//    MatrixXd node_path_counts = compute_top_paths(nodes_of_type, config.max_num_paths);
    set<size_t> single_nodes;
    vector<set<size_t>> clusters;

    if (hypothesis_test_path_symmetric_nodes(nodes_of_type, number_of_walks, config.max_num_paths, length_of_walks, config.theta_p)) {
        set<size_t> cluster;
        for(auto node:nodes_of_type){
            cluster.insert(node.get_node_id());
        }
        clusters.emplace_back(cluster);
    }else{
//        if(nodes_of_type.size() <= config.clustering_method_threshold){
            pair<set<size_t>, vector<set<size_t>>> sk_clusters = cluster_nodes_by_sk_divergence(nodes_of_type,
                                                                                                config.theta_p,
                                                                                                number_of_walks,
                                                                                                config.max_num_paths);
            single_nodes = sk_clusters.first; // TODO Check for potential memory leakage
            clusters = sk_clusters.second;
//        }else{
//            TODO implement birch
//        }
    }
    return {single_nodes, clusters};
}

MatrixXd compute_top_paths(const vector<NodeRandomWalkData> &nodes_of_type, size_t max_number_of_paths, size_t path_length) {
    vector<vector<pair<string, int>>> top_paths_of_each_node;
    for (auto node: nodes_of_type) {
        top_paths_of_each_node.emplace_back(node.get_top_paths(max_number_of_paths, path_length));
    }
    set<string> unique_paths;
    for (auto paths: top_paths_of_each_node) {
        for (auto path_pair: paths) {
            unique_paths.insert(path_pair.first);
        }
    }

    size_t number_of_unique_paths;
    number_of_unique_paths = unique_paths.size();
    unique_paths.clear();

    MatrixXd node_path_counts = MatrixXd::Zero(number_of_unique_paths, nodes_of_type.size());
    if (number_of_unique_paths > 0) {
        map<string, size_t> path_string_to_path_index;
        size_t node_index{0};
        for (auto node_paths: top_paths_of_each_node) {
            for (auto path: node_paths) {
                size_t path_index{};
                if (!has(get_keys(path_string_to_path_index), path.first)) {
                    path_index = path_string_to_path_index.size();
                    path_string_to_path_index[path.first] = path_index;
                } else {
                    path_index = path_string_to_path_index[path.first];
                }
                node_path_counts(path_index, node_index) = path.second;
            }
            node_index++;
        }
    }

    return node_path_counts;

}

pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_sk_divergence(const vector<NodeRandomWalkData> &nodes_of_type,
                                                                      double significance_level,
                                                                      size_t number_of_walks,
                                                                      size_t max_number_of_paths) {
    // TODO improve efficiency of emplace back
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

    set<size_t> single_nodes;
    vector<set<size_t>> clusters;
    for (auto sk_cluster: sk_clusters) {
        if (sk_cluster.number_of_nodes() == 1) {
            size_t node_id = *sk_cluster.get_node_ids().begin();
            single_nodes.insert(node_id);
        } else {
            clusters.emplace_back(sk_cluster.get_node_ids());
        }
    }

    return {single_nodes, clusters};
}



pair<set<size_t>, vector<set<size_t>>> cluster_nodes_by_birch(const vector<NodeRandomWalkData> &nodes,
                                                              int pca_target_dimension,
                                                              int max_number_of_paths,
                                                              int number_of_walks,
                                                              float significance_level) {
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

    vector<size_t> clustering_labels = compute_optimal_birch_clustering(node_path_counts,
                                                                        pca_target_dimension,
                                                                        number_of_walks,
                                                                        significance_level);


    return group_nodes_by_clustering_labels(nodes, clustering_labels);
}

vector<size_t> compute_optimal_birch_clustering(MatrixXd node_path_counts,
                                                int pca_target_dimension,
                                                int number_of_walks,
                                                float significance_level){
    /*
     * Given an array of node path counts, clusters the nodes into an optimal number of clusters using birch clustering.
     * The number of clusters is incrementally increased. The optimal number of clusters is the smallest number of clusters
     * such that have statistically similar path count distributions at a specified significance level.
     * */
    VectorXd mean_vector;
    mean_vector = node_path_counts.rowwise().mean();
    MatrixXd standardized_path_counts;
    standardized_path_counts =  mean_vector.inverse().asDiagonal() * (node_path_counts - mean_vector); // TODO CHeck this is what we want, Dom!!!
    MatrixXd feature_vectors = compute_principal_components(standardized_path_counts,
                                                            pca_target_dimension);

   int number_of_feature_vectors = feature_vectors.rows();

   vector<size_t> cluster_labels;
    for(int i=2; i < number_of_feature_vectors; i++) { // start from 2 since zero/one clusters is invalid

    }
//    clusterer = Birch(n_clusters = number_of_clusters, threshold = 0.05);

    cluster_labels = clusterer.fit_predict(feature_vectors);
    node_path_counts_of_clusters = get_node_path_counts_of_clusters(node_path_counts, cluster_labels);
    if (test_quality_of_clusters(node_path_counts_of_clusters, number_of_walks, significance_level)) {
        return cluster_labels;
    } else {
        continue;
    }
}
    }

    return cluster_labels;

}

MatrixXd compute_principal_components(MatrixXd feature_vectors, int target_dimension){
    //Dimensionality reduces feature vectors into a target dimension using Principal Component Analysis.
    //
    //:param feature_vectors: (number_of_feature_vectors) x (dimension_of_feature_vectors)
    //:param target_dimension: the desired dimension of the dimensionality-reduced data
    //:return: principal_components: the dimensionality-reduced feature vectors

    size_t original_dimension = feature_vectors.cols();
    if (original_dimension > target_dimension){
        return PCA(feature_vectors, target_dimension);
    }
    return feature_vectors;
}



pair<set<size_t>, vector<set<size_t>>> group_nodes_by_clustering_labels(const vector<NodeRandomWalkData> &nodes,
                                                                        vector<size_t> cluster_labels){
    //Groups a list of nodes into single nodes and clusters from a list of cluster labels.
    //
    //:param nodes: the nodes to be grouped
    //:param cluster_labels: a list of integers assigning each node to a given cluster

//    number_of_clusters = len(set(cluster_labels))
//    original_clusters = [[] for _ in range(number_of_clusters)]
//    for node_index, cluster_index in enumerate(cluster_labels):
//    original_clusters[cluster_index].append(nodes[node_index].name)
//
//    // split into single nodes and clusters
//    single_nodes = set()
//    clusters = []
//    for cluster in original_clusters:
//    if len(cluster) == 1:
//        node_name = cluster[0]
//        single_nodes.add(node_name)
//    else:
//        clusters.append(cluster)

    return pair<set<size_t>, vector<set<size_t>>>();
}

void two_means(MatrixXd &all_points, int max_iterations, double threshold) //Each column is a point
{
    double epsilon = 0.00000001;
    all_points += Eigen::MatrixXd::Ones(all_points.rows(), all_points.cols()) * epsilon;  // to avoid zero-value related errors
    int total_points = all_points.cols();
    int dimensions = all_points.rows();

    //Check cluster quality of all_points
    //if good, return all_points
    //else
    //call 2means


    // initialze 2 means randomly
    // for each point
    // calculate euclidean distance (point, mean1), (point, mean2)
    // assign to closer one
    //  compute new means
    // if distance oldmean-newmean<threshold OR Max loops
    // break
    // else continue

    // Initializing Clusters
    vector<int> used_pointIds;
    VectorXd centroid1 = all_points.col(0);
    VectorXd centroid2 = all_points.col(1);
    int iter{0};
    vector<int> cluster_association(total_points);

    while(true){
        MatrixXd cluster1 = Eigen::MatrixXd::Zeros(all_points.rows(), all_points.cols());
        MatrixXd cluster2 = Eigen::MatrixXd::Zeros(all_points.rows(), all_points.cols());
        bool converged = false;
        for(int i{0}; i<total_points; i++){
            //assign i to nearest cluster;
            VectorXd displacement_to_1 = centroid1 - all_points.col(i);
            VectorXd displacement_to_2 = centroid2 - all_points.col(i);
            int distance1 = displacement_to_1.dot(displacement_to_1);
            int distance2 = displacement_to_2.dot(displacement_to_2);
            if(distance1 <distance2){
                cluster_association[i] = 0;
                cluster1.col(i) = all_points.col(i);
            }else{
                cluster_association[i] = 1;
                cluster2.col(i) = all_points.col(i);
            }

        }
        //compute new centroid
        VectorXd new_centroid1;
        new_centroid1 = cluster1.rowwise().nonZeros().mean();
        VectorXd new_centroid2;
        new_centroid2 = cluster2.rowwise().nonZeros().mean();

        double distance = pow((new_centroid1 - centroid1).dot(new_centroid1 -centroid1) + (new_centroid2 - centroid2).dot(new_centroid2 -centroid2),0.5);
        //determine convergence
        if(distance < threshold){
            converged = true;
        }
        if (converged || iter >= max_iterations)
        {
            break;
        }
        iter++;
    }
        return cluster_association;

}