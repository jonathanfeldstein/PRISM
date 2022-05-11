//
// Created by jonathan on 03/05/2022.
//

#include "read_files_utils.h"

vector<HyperGraph> construct_hypergraphs_from_files(string db_file_path, string info_file_path, bool safe){
    vector<HyperGraph> constructed_hypergraphs;
    HyperGraph original_hypergraph(db_file_path, info_file_path, safe);
    if(original_hypergraph.is_connected()){
        constructed_hypergraphs.emplace_back(original_hypergraph);
        return constructed_hypergraphs;
    }else{
        vector<set<NodeId>> unconnected_components = original_hypergraph.find_unconnected_components();
        for(auto component:unconnected_components){
            HyperGraph subgraph(component, original_hypergraph);
            constructed_hypergraphs.emplace_back(subgraph);
        }
        return constructed_hypergraphs;
    }
    
}

Relation parse_line_info(std::string line, bool safe) {
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); //Strip White Space
    if(safe){
        std::regex relation_expression_format("[0-9a-zA-Z_]+\\([0-9a-zA-Z_]+(,[0-9a-zA-Z_]+)*\\):?((0(\\.\\d+)?|1(\\.0+)?))?$");
        if(!std::regex_match(line, relation_expression_format)){
            throw DatabaseSyntaxException(line);
        }
    }
    size_t pos = 0;
    Relation relation;
    while ((pos = line.find_first_of(",()")) != std::string::npos) { //Split the line by ',' or '(' or ')'
        if (relation.predicate.empty()){
            relation.predicate = line.substr(0, pos);
        }else{
            relation.arguments.push_back(line.substr(0, pos));
        }
        line.erase(0, pos + 1);
    }
    return relation;
}

GroundRelation parse_line_db(std::string line, bool safe) {
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); //Strip White Space
    if(safe){
        std::regex relation_expression_format("[0-9a-zA-Z_]+\\([0-9a-zA-Z_]+(,[0-9a-zA-Z_]+)*\\):?((0(\\.\\d+)?|1(\\.0+)?))?$");
        if(!std::regex_match(line, relation_expression_format)){
            throw DatabaseSyntaxException(line);
        }
    }
    size_t pos = 0;
    GroundRelation relation;
    relation.weight = 1.0;
    while ((pos = line.find_first_of(",():")) != std::string::npos) { //Split the line by ',' or '(' or ')'
        if (relation.predicate.empty()){
            relation.predicate = line.substr(0, pos);
        }else if(line[pos] == ':'){
            std::string weight_string = line.substr(pos+1, std::string::npos);
            if(!weight_string.empty()){
                relation.weight = stod(weight_string);
            }
        }else{
            relation.arguments.push_back(line.substr(0, pos));
        }
        line.erase(0, pos + 1);
    }
    return relation;
}

bool file_exists(const std::string& path_to_file) {
    struct stat buffer;
    return (stat (path_to_file.c_str(), &buffer) == 0);
}