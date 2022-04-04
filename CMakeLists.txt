cmake_minimum_required(VERSION 3.16)
project(FASTER)

set(CMAKE_CXX_STANDARD 20)

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
        Communities/CommunityPrinter.cpp Communities/CommunityPrinter.h Utils/vector_utils.cpp Utils/vector_utils.h)

