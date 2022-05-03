//
// Created by jonathan on 03/05/2022.
//

#ifndef FASTER_READ_FILES_UTILS_H
#define FASTER_READ_FILES_UTILS_H

#include <string>
#include <algorithm>

#include "../Graphs/Relation.h"


GroundRelation parse_line_db(std::string line);
Relation parse_line_info(std::string line);

#endif //FASTER_READ_FILES_UTILS_H
