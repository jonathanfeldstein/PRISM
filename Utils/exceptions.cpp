//
// Created by jonathan on 03/05/2022.
//

#include "exceptions.h"


FileNotFoundException::FileNotFoundException(const std::string &filename) noexcept : filename(filename){
    this->message_ = "File not found: " + filename;
}

FileNotOpenedException::FileNotOpenedException(const std::string &filename) noexcept {
    this->message_ = "File did not open correctly: " + filename;
}

CommunitiesGenerationException::CommunitiesGenerationException(std::size_t number_of_communities, std::size_t number_of_nodes) noexcept {
    this->message_ = "Incorrect hypergraph provided for original_hypergraph. More communities found ("
            + std::to_string(number_of_communities) + ") than the number of nodes in original_hypergraph ("
            + std::to_string(number_of_nodes) + ").";
}

RW_InvalidArgumentException::RW_InvalidArgumentException(const std::string &parameter) noexcept {
    this->message_ = "Invalid arguments put into RANDOM WALKER config: " + parameter + " failed criteria. See RandomWalker.h -> RandomWalkerConfig";
}


