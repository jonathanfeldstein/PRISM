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
// CONFIG EXCEPTIONS
////////////////////////////////////////////////////////

struct InvalidArgumentException : public std::exception{
    const char* what () const throw (){
        return "Invalid argument put into config";
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
