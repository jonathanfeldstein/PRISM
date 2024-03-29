# PRISM 1.0.0
**PR**incipled **I**dentification of **S**tructural **M**otif is a framework for identifying structural motifs in relational data. This is an implementation of the algorithms described in the paper *"Principled and Efficient Motif Finding for Structure Learning in Lifted Graphical Models" by Jonathan Feldstein, Dominic Phillips and Efthymia Tsamoura, AAAI 2023*. It is implemented in C++ and highly parallelized to allow for faster computations.

You can read the original paper [here](PRISM_AAAI23_arXiv.pdf). (https://arxiv.org/abs/2302.04599)

![alt text](structureLearning.png "Structure Learning Pipeline")

**Structure Learning Pipeline:** The above shows an example of a dataset about a class, where entities are people or books. Black edges represent $TEACHES(X,Y)$, and red edges represent $READS(Y,Z)$: (i) The resulting higher-level concepts, where the top concepts represent teachers, the middle concepts represent students, and the bottom concepts are books (ii) the resulting structural motif, (iii) the paths found in the motif (in this case two), and (iv) mined candidate clauses.

This README should provide you the necessary understanding to run examples using this library and build upon it further. If you do use this library, please cite accordingly.
This README consists of the following sections:

1. [A brief technical overview](#a-brief-technical-overview)
2. [The library structure](#the-library-structure)
3. [How to install PRISM?](#how-to-install-PRISM)
4. [How to run the examples?](#how-to-run-the-examples)
5. [How to set up your own experiments?](#how-to-set-up-your-own-experiments)

## A brief technical overview

In this section, we briefly describe what PRISM can be used for and the underlying technical steps that are used to find structural motifs. For a detailed description and a more fundamental mathematical understanding, please consult *"Principled and Efficient Motif Finding for Structure Learning in Lifted Graphical Models" by Jonathan Feldstein, Dominic Phillips and Efthymia Tsamoura, AAAI 2023*.

**Structure learning** is a core problem in AI central to the fields of **neuro-symbolic AI** and **statistical relational learning**. It consists in automatically learning a logical theory from data. The basis for structure learning is mining repeating patterns in the data, known as **structural motifs**. Finding these patterns reduces the exponential search space and therefore guides the learning of formulas. Despite the importance of motif learning, it is still not well understood. We present the first principled approach for mining structural motifs in **lifted graphical models**, languages that blend first-order logic with probabilistic models, which uses a stochastic process to measure the similarity of entities in the data.

**PRISM** takes the following steps, illustrated in the figure below to find structural motifs

![alt text](ConceptFinding.png "Concept Finding Pipeline")

#### 1. Transform data into a hypergraph

The input to PRISM is data in form of a relational database. A **relational database** $\mathcal{D}$ can be represented by a hypergraph $\mathcal{H} = (V,E)$ by defining $V$ to be the union of the constants in $\mathcal{D}$, and defining $E$ such that every $k$ -ary ground atom $R(c_1,\dots,c_k)$ in $\mathcal{D}$ becomes a hyperedge $e \in E$, with label $R$, whose elements are the nodes corresponding to the constants $c_1,\dots,c_n$.  

#### 2. Perform hierarchical clustering of the hypergraph

We then pre-process the data through hierarchical clustering. Illustrated in the figure above (Step (i)). This identifies groups of densely-connected nodes. The motivation behind considering only densely-connected nodes is that spuriously-connected nodes appear rarely in the path signatures and therefore only add noise to the path signature counts that we use at a later stage for clustering nodes into *abstract concepts*. Furthermore, this pre-processing also leads to a computational saving as we show in our paper.

This hierarchical clustering algorithm is based on spectral clustering. 
In overview, we begin by converting a hypergraph $\mathcal{H}=(V,E)$ into a weighted graph $\mathcal{G}$ by expanding cliques over each hyperedge. Next, $\mathcal{G}$ is recursively bipartitioned using the sweep set approximation algorithm for the Cheeger-cut *(Chang, Shao, and Zhang 2017)*. The result of the partitioning is a set of subgraphs $G:=$ { $\mathcal{G}_1, \mathcal{G}_2, ..., \mathcal{G}_k$ }. The partitioning terminates whenever either the second-smallest eigenvalue of the symmetric Laplacian matrix $\lambda_2$ exceeds a threshold value $\lambda_2^{max}$.
$\lambda_2^{max}$ is dataset independent and thus fixed in our implementation.
Finally, each graph cluster $\mathcal{G}_i$ is then converted into a hypergraph $\mathcal{H}_i = (V_i, E_i)$ such that the vertex set $V_i$ of the hypergraph is initialised to be the vertex set of $\mathcal{G}_i$. The edge set $E_i$ is then constructed by adding all hyperedges $e \in E$ whose strict majority of element vertices appear in $V_i$, i.e. $E_i :=$ { $e \in E \vert \vert e \cap V_i \vert > \vert e\vert/2$}. As a consequence, no nodes nor edges are lost during clustering. 
This algorithm returns the set of hypergraph clusters { $\mathcal{H}_1, \mathcal{H}_2, ..., \mathcal{H}_k$ } obtained. After partitioning, we run the rest of the pipeline with $L$ set to the diameter of each hypergraph cluster.

#### 3. Run random walks from each node in the hypergraph

In a next step, we run $N$ random walks from each node. $N$ is computed given a user-defined $\varepsilon$. During each random walk, we store for each node that we encounter during the random walk the path signature of the path taken to reach the node for the first time from the source node of the random walk.

An upper bound on the $\varepsilon$-optimal number of random walks $N$ on $\mathcal{H}$ under $L$ is given by 

$\max$ { $(L-1)^2/4\varepsilon^2, P^\* (\gamma + \ln P^\*)/\varepsilon^2$ }, 

where ${P^\* = 1 + {e(e^{L}-1)}/({e-1}) \gg 1}$, $e$ is the number of unique edge labels in $\mathcal{H}$, and $\gamma \approx 0.577$ is the Euler-Mascheroni constant.

#### 4. Cluster nodes into *abstract concepts*

Finally, we cluster nodes in *abstract concepts*; collections of entities that have similar neighbourhoods in the hypergraph. Illustrated in the figure above (Step (ii) and (iii)). Nodes are then partitioned into path-symmetric sets based on the similarity of their $L$-path signature counts. Each path-symmetric set then uniquely defines an abstract concept.

In practice, we separate the path-symmetry clustering into two steps. Firstly, we cluster nodes based on their distance symmetry and then based on their path symmetry. The stage of clustering by distance only serves to speed up the subsequent path-symmetric clustering. This is a valid approach since any path-symmetry implies distance symmetry, and checking distance symmetry is a faster algorithm ( $\mathcal{O}(N)$ vs $\mathcal{O}(N \ln (N))$ ).

## The library structure

#### Graphs

`./Graphs` contains a light C++ implemented graph library supporting both graphs and hypergraphs. 

##### `UndirectedGraph`

Our `UndirectedGraph` class is implemented as a child class of the BOOST graph library. We extended the members by

```
     Graph graph;
     property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, graph);
     MatrixXd adjacency_matrix;
     MatrixXd degree_matrix;
     MatrixXd laplacian_matrix;
     MatrixXd sqrt_degree;
     bool diameter_computed = false;
     size_t diameter{0};
```

Further, we implemented the following methods (further methods are implemented but meant to be used only as subfunctions of the methods described here):

`UndirectedGraph::UndirectedGraph(HyperGraph &hypergraph) : graph(hypergraph.number_of_nodes())` a method that allows to implement a graph from a `HyperGraph` object. This achieved by extending each hyperedge in the original `HyperGraph` by a clique of edges connecting each node in the hyperedge.

`UndirectedGraph::UndirectedGraph(UndirectedGraph &graph_template, set<NodeId> subgraph_nodes)`

`pair<UndirectedGraph, UndirectedGraph> UndirectedGraph::cheeger_cut(VectorXd &second_EV)`

Otherwise, we have implemented a few getter and setter functions to access and modify the different private members.

##### `HyperGraph`

Our `HyperGraph` class is an implementation of a hypergraph implemented from scratch as no library in C++ was available. It contains the following members:

```
    map<EdgeId, vector<NodeId>> edges; 
    map<EdgeId, double> edge_weights; 
    map<EdgeId, Predicate> predicates; 
    map<NodeId, NodeName> node_ids_names; 
    map<NodeName, NodeId> node_names_ids; 
    map<NodeId, NodeType> nodes; 
    map<NodeId, vector<EdgeId>> memberships; 
    map<Predicate, vector<NodeType>> predicate_argument_types; 
    set<NodeType> node_types; 
    size_t estimated_graph_diameter{0};
    map<NodeId, bool> is_source_node; 
```

Three constructors are available to instantiate a hypergraph

Firstly, we allow to instantiate a hypergraph from files.

```cpp
HyperGraph(string const& db_file_path, string const& info_file_path, bool safe);
```

Secondly, we allow to instantiate a hypergraph from an UndirectedGraph object and a template. 
This method is used to transform back a graph to a hypergraph after performing hierarchical clustering on the graph that was obtained by extending each hyperedge in an original hypergraph by a clique.

```cpp
HyperGraph(UndirectedGraph &graph, HyperGraph &hypergraph_template);  
```

Thirdly, we allow to instantiate a hypergraph from a set of nodes with a template describing the edges. 
```cpp
HyperGraph(set<NodeId> nodes_subset, HyperGraph &hypergraph_template);
```

##### `HierarchicalClusterer`

The `HierarchicalClusterer` class is our implementation of a recursive spectral clustering. 

Our constructor instantiates an object given a hypergraph to be clustered, and a config file.

```cpp
HierarchicalClusterer(HyperGraph hypergraph, HierarchicalClustererConfig config); 
```  

The main method to perform the hierarchical clustering is

```cpp
vector<HyperGraph> & run_hierarchical_clustering();  
```


##### `Relation`

Is a header file for three auxiliary structs, namely

```cpp
struct GroundRelation;
struct Relation;
struct NodePartition;
```

#### RandomWalker

The RandomWalker folder contains all classes and functions to run random walks and collect the data on path signatures and their counts.

#### Concepts

The Concepts folder contains all classes to store the identified abstract concepts and print them into files for further usage in other frameworks.

#### Utils

The Utils folder contains a plethora of auxiliary methods including to compute statistics, allow operations on sets, vectors and maps and much more.

#### UnitTests

The UnitTests folders contains all tests to check the correctness of the code.

## How to install PRISM?

PRISM needs three libraries pre-installed to run:

* [BOOST](https://www.boost.org/): A C++ library we use for their graph implementation.

* [EIGEN](https://eigen.tuxfamily.org/index.php?title=Main_Page): A C++ library for matrix manipulation.

* [SPECTRA](https://spectralib.org/): A C++ library for fast eigen decompositions.

Once these three libraries are installed, it remains to download this library, change the paths in the [CMakeLists](CMakeLists.txt) file to the installation paths of the respective libraries and build the library.

To compile the library:
1. Check that your system has cmake version 3.16 or later (Installation instructions: https://cmake.org/install/)
2. Once CMake is installed, you can use it from the command line by opening a terminal or command prompt and navigating to the directory where your CMakeLists.txt file is located.

To generate build files for your project using CMake, you can use the following command:

```
cmake -S . -B cmake-build-debug -DBOOST_PATH="/path/to/boost_1_78_0" -DEIGEN_PATH="/path/to/eigen-3.4.0" -DSPECTRA_PATH="/path/to/spectra/include"
```
Then build the program by
```
cmake --build cmake-build-debug
```
You should then be able to run the code by using

```
cd cmake-build-debug
./PRISM ../Datasets/imdb1.db ../Datasets/imdb1.info ../Datasets/imdb_output 0.1 0.01 1 0 
```
This runs PRISM on a small IMDB test dataset using the following hyperparameters:
- `../Datasets/imdb1.db` is the file path to the database
- `../Datasets/imdb1.info` is the file path to the info file describing the different predicates
- `../Datasets/imdb_output` is the file path to the different generated outputs.
- $\epsilon = 0.1$
- $\alpha = 0.01$
- `multiprocessing = True`
- `verbose = False`
 
## How to set up your own experiments?

To run your own experiments, simply add `.db` and `.info` files akin to the `imdb1.db` and `imdb1.info` to the `Datasets` folder. 
Rerun the command above with the corrected paths and changed parameters as required.
