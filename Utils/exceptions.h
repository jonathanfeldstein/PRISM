//
// Created by jonathan on 03/05/2022.
//

#ifndef FASTER_EXCEPTIONS_H
#define FASTER_EXCEPTIONS_H

#include <string>
#include <exception>
#include <iostream>
////////////////////////////////////////////////////////
// HYPERGRAPH EXCEPTIONS
////////////////////////////////////////////////////////
struct HyperGraphEmptyException : public std::exception{
    const char* what () const throw (){
        return "Hypergraph is empty";
    }
};

struct HyperGraphSizeException : public std::exception{
    const char* what () const throw (){
        return "Hypergraph is too small (< 3 nodes)";
    }
};

struct HyperGraphConnectedException : public std::exception{
    const char* what () const throw (){
        return "Hypergraph is not connected";
    }
};

////////////////////////////////////////////////////////
// COMMUNITIES EXCEPTIONS
////////////////////////////////////////////////////////
struct CommunitiesGenerationException : public std::exception{
private:
    std::string message_;
public:
    explicit CommunitiesGenerationException(std::size_t number_of_communities, std::size_t number_of_nodes) noexcept;
    virtual ~CommunitiesGenerationException() = default;
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
};
////////////////////////////////////////////////////////
// CONFIG EXCEPTIONS
////////////////////////////////////////////////////////

struct HC_InvalidArgumentException : public std::exception{
    const char* what () const throw (){
        return "Invalid arguments put into HIERARCHICAL CLUSTERING config, following must be satisfied: config.min_cluster_size > 2 && 0 <= config.max_lambda2 && config.max_lambda2 <= 2";
    }
};

struct RW_InvalidArgumentException : public std::exception{
private:
    std::string message_;
    std::string parameter;
public:
    explicit RW_InvalidArgumentException(const std::string& parameter) noexcept;
    virtual ~RW_InvalidArgumentException() = default;
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
};

////////////////////////////////////////////////////////
// DATABASE EXCEPTIONS
////////////////////////////////////////////////////////

struct FileNotFoundException : public std::exception{
private:
    std::string message_;
    std::string filename;
public:
    explicit FileNotFoundException(const std::string& filename) noexcept;
    virtual ~FileNotFoundException() = default;
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
};

struct FileNotOpenedException : public std::exception{
private:
    std::string message_;
    std::string filename;
public:
    explicit FileNotOpenedException(const std::string& filename) noexcept;
    virtual ~FileNotOpenedException() = default;
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
};


struct DatabaseSyntaxException : public std::exception{
    const char* what () const throw (){
        return "Database syntax incorrect";
    }
};
#endif //FASTER_EXCEPTIONS_H
