cmake_minimum_required(VERSION 3.16)
project(FASTER)




# openmp
find_package(OpenMP)
if(OPENMP_FOUND)
    message("OpenMP Found")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 20)

include_directories(SYSTEM ${BOOST_PATH} ./Communities ./Graphs ./RandomWalker ./Utils ./UnitTests ${EIGEN_PATH} ${SPECTRA_PATH})

add_executable(FASTER main.cpp
        Graphs/UndirectedGraph.cpp Graphs/UndirectedGraph.h
        Graphs/HyperGraph.cpp Graphs/HyperGraph.h
        Graphs/HierarchicalClusterer.cpp Graphs/HierarchicalClusterer.h
        RandomWalker/RandomWalker.cpp RandomWalker/RandomWalker.h
        RandomWalker/NodeRandomWalkData.cpp RandomWalker/NodeRandomWalkData.h
        RandomWalker/NodeClusterRandomWalkData.cpp RandomWalker/NodeClusterRandomWalkData.h
        Communities/Community.cpp Communities/Community.h
        Communities/Communities.cpp Communities/Communities.h
        Communities/CommunityPrinter.cpp Communities/CommunityPrinter.h
        Utils/vector_utils.cpp Utils/vector_utils.h
        Utils/map_utils.cpp Utils/map_utils.h
        Utils/random_utils.cpp Utils/random_utils.h
        Utils/set_utils.cpp Utils/set_utils.h
        Utils/clustering_utils.cpp Utils/clustering_utils.h
        Utils/hypothesis_testing.cpp Utils/hypothesis_testing.h
        Utils/sk_divergence_utils.cpp Utils/sk_divergence_utils.h
        Utils/stats_utils.cpp Utils/stats_utils.h
        Utils/profiling_utils.cpp Utils/profiling_utils.h
        Utils/pca_utils.cpp Utils/pca_utils.h
        UnitTests/test_graphs.cpp UnitTests/test_graphs.h
        UnitTests/test_random_walks.cpp UnitTests/test_random_walks.h
        UnitTests/test_hierarchical_clustering.cpp UnitTests/test_hierarchical_clustering.h
        UnitTests/test_communities.cpp UnitTests/test_communities.h
        UnitTests/test_statistics.cpp UnitTests/test_statistics.h
        UnitTests/run_all_tests.cpp UnitTests/run_all_tests.h Utils/read_files_utils.cpp Utils/read_files_utils.h Graphs/Relation.h Utils/exceptions.h Utils/exceptions.cpp)
