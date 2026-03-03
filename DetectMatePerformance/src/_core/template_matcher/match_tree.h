#ifndef M_TREE_H  
#define M_TREE_H

#include <vector>
#include <utility> 
#include <string>
#include <deque>

#include "tree.h"
#include "../_type/templates.h"
#include "../_type/parsed.h"


class MatchTree {
private:
    Tree* tree;
    Templates* templates;

public:
    MatchTree(Templates* templates);

    ~MatchTree();

    ParsedMessages* match_string(std::string sentence);

    ParsedMessages* match_string_with_var(std::string sentence);

    ParsedMessages* match_batch(std::vector<std::string> sentences, int n_workers);

    std::pair<std::vector<std::string>, std::vector<std::deque<std::string>>> match_batch_with_var(
        std::vector<std::string> sentences, int n_workers
    );

    bool isEqual(Tree* other_tree);

    bool isEqual(MatchTree* matcher);

    Templates* getTemplates();
};
#endif 