#ifndef M_AUTOPARSER_H
#define M_AUTOPARSER_H

#include <vector>
#include <string>

#include "../_type/templates.h"
#include "../template_matcher/match_tree.h"


std::pair<Templates, int> autoParserGenerator(
    std::vector<std::string> sentences,
    std::vector<std::string> templatePaths,
    std::vector<const char*> regexs
);


std::pair<Templates, int> doAutoParse(
    std::vector<std::string> sentences,
    std::string pathTemplates
);


#endif
