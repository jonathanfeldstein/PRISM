cmake_minimum_required(VERSION 3.16)
project(FASTER)

# openmp
find_package(OpenMP)
if(OPENMP_FOUND)
    message("OpenMP Found")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Werror -Wl,-rpath,$ORIGIN")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g")

include_directories(SYSTEM /home/jonathan/Applications/boost_1_66_0 ./Communities ./Graphs ./RandomWalker ./Utils /home/jonathan/Applications/eigen-3.4.0)

add_executable(FASTER main.cpp
        Graphs/UndirectedGraph.cpp Graphs/UndirectedGraph.h
        Graphs/HyperGraph.cpp Graphs/HyperGraph.h
        Graphs/HierarchicalClusterer.cpp Graphs/HierarchicalClusterer.h
        Utils/map_utils.cpp Utils/map_utils.h
        RandomWalker/RandomWalker.cpp RandomWalker/RandomWalker.h
        RandomWalker/NodeRandomWalkData.cpp RandomWalker/NodeRandomWalkData.h
        RandomWalker/NodeClusterRandomWalkData.cpp RandomWalker/NodeClusterRandomWalkData.h
        Communities/Community.cpp Communities/Community.h
        Communities/Communities.cpp Communities/Communities.h
        Communities/CommunityPrinter.cpp Communities/CommunityPrinter.h Utils/vector_utils.cpp Utils/vector_utils.h Utils/random_utils.cpp Utils/random_utils.h Utils/set_utils.cpp Utils/set_utils.h Utils/clustering_utils.cpp Utils/clustering_utils.h Utils/hypothesis_testing.cpp Utils/hypothesis_testing.h Utils/sk_divergence_utils.cpp Utils/sk_divergence_utils.h Utils/stats_utils.cpp Utils/stats_utils.h Utils/profiling_utils.cpp Utils/profiling_utils.h)

