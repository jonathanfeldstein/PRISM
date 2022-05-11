//
// Created by jonathan on 03/05/2022.
//

#ifndef FASTER_READ_FILES_UTILS_H
#define FASTER_READ_FILES_UTILS_H

#include <string>
#include <algorithm>
#include <sys/stat.h>
#include <regex>
#include "exceptions.h"

#include "../Graphs/Relation.h"

GroundRelation parse_line_db(std::string line, bool safe);
Relation parse_line_info(std::string line, bool safe);
bool file_exists(const std::string& path_to_file);

#endif //FASTER_READ_FILES_UTILS_H
