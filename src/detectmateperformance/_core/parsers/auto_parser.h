#ifndef M_AUTOPARSER_H
#define M_AUTOPARSER_H

#include <vector>
#include <string>

#include "../_type/templates.h"
#include "../template_matcher/match_tree.h"


Templates auto_parser_generator(std::vector<std::string> sentences, std::vector<std::string> templatePaths,std::vector<std::string> regexs);

#endif
