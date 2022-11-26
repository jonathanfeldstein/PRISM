# PRISM
**PR**incipled **I**dentification of **S**tructural **M**otif is a framework for identifying structural motifs in relational data. This is an implementation of the algorithms described in the paper *"Principled and Efficient Motif Finding for Structure Learning in Lifted Graphical Models" by Jonathan Feldstein, Dominic Phillips and Efthymia Tsamoura, AAAI 2023*. It is implemented in C++ and highly parallelized to allow for faster computations.

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

**PRISM** takes the following steps to find structural motifs:

### 1. Transform data into a hypergraph

The input to PRISM is data in form of a relational database. A **relational database** ![equation](https://latex.codecogs.com/svg.image?\mathcal{D}) can be represented by a hypergraph ![equation](https://latex.codecogs.com/svg.image?\mathcal{H}&space;=&space;(V,E)) by defining ![equation](https://latex.codecogs.com/svg.image?V) to be the union of the constants in ![equation](https://latex.codecogs.com/svg.image?\mathcal{D}), and defining ![equation](https://latex.codecogs.com/svg.image?E) such that every ![equation](https://latex.codecogs.com/svg.image?k) -ary ground atom ![equation](https://latex.codecogs.com/svg.image?R(c_1,\dots,c_k)) in ![equation](https://latex.codecogs.com/svg.image?\mathcal{D}) becomes a hyperedge ![equation](https://latex.codecogs.com/svg.image?e&space;\in&space;E), with label ![equation](https://latex.codecogs.com/svg.image?R), whose elements are the nodes corresponding to the constants ![equation](https://latex.codecogs.com/svg.image?c_1,\dots,c_n).  
### 2. Perform hierarchical clustering of the hypergraph
We then pre-process the data through hierarchical clustering. This identifies groups of densely-connected nodes. The motivation behind considering only densely-connected nodes is that spuriously-connected nodes appear rarely in the path signatures and therefore only add noise to the path signature counts that we use at a later stage for clustering nodes into *abstract concepts*. Furthermore, this pre-processing also leads to a computational saving as we show in our paper.

The algorithm $\hcluster$ is based on spectral clustering. A discussion of spectral clustering is beyond the scope of this paper, but details are provided in the Appendix along with pseudocode. 

In overview, $\hcluster$ begins by converting a hypergraph ![equation](https://latex.codecogs.com/svg.image?\mathcal{H}&space;=&space;(V,E)) into a weighted graph ![equation](https://latex.codecogs.com/svg.image?\mathcal{G}) by expanding cliques over each hyperedge. 
Next, ![equation](https://latex.codecogs.com/svg.image?\mathcal{G}) is recursively bipartitioned using the sweep set approximation algorithm for the Cheeger-cut *(Chang, Shao, and Zhang 2017)*. The result of the partitioning is a set of subgraphs ![equation](https://latex.codecogs.com/svg.image?\mathcal{G}:=\{\mathcal{G}_1,\mathcal{G}_2,&space;...,\mathcal{G}_k\}). The partitioning terminates whenever either the second-smallest eigenvalue of the symmetric Laplacian matrix ![equation](https://latex.codecogs.com/svg.image?\lambda_2) exceeds a threshold value ![equation](https://latex.codecogs.com/svg.image?\lambda_2^{max}).
![equation](https://latex.codecogs.com/svg.image?\lambda_2^{max}) is dataset independent and thus fixed in our implementation.
Finally, each graph cluster ![equation](https://latex.codecogs.com/svg.image?\mathcal{G}_i) is then converted into a hypergraph ![equation](https://latex.codecogs.com/svg.image?\mathcal{H}_i&space;=&space;(V_i,E_i)) such that the vertex set ![equation](https://latex.codecogs.com/svg.image?V_i) of the hypergraph is initialised to be the vertex set of ![equation](https://latex.codecogs.com/svg.image?\mathcal{G}_i). The edge set ![equation](https://latex.codecogs.com/svg.image?E_i) is then constructed by adding all hyperedges ![equation](https://latex.codecogs.com/svg.image?e\in&space;E) whose strict majority of element vertices appear in ![equation](https://latex.codecogs.com/svg.image?V_i), i.e. ![equation](https://latex.codecogs.com/svg.image?E_i&space;:=\left\{\right\}\left\{&space;\right\}). As a consequence, no nodes nor edges are lost during clustering.
$\hcluster$ returns the set of hypergraph clusters $\{\mathcal{H}_1, \mathcal{H}_2, ..., \mathcal{H}_k\}$ obtained. After partitioning, we run the rest of the pipeline with $L$ set to the diameter of each hypergraph cluster.
### 3. Run random walks from each node in the hypergraph
### 4. Cluster nodes into *abstract concepts*

## The library structure

## How to install PRISM?

## How to run the examples?

## How to set up your own experiments?