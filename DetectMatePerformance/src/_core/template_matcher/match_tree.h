#ifndef M_TREE_H  
#define M_TREE_H

#include <vector>
#include <utility> 
#include <string>
#include <deque>

#include "tree.h"
#include "../_type/templates.h"


class MatchTree {
private:
    Tree* tree;

public:
    MatchTree(Templates* templates);

    ~MatchTree();

    std::string match_string(std::string sentence);

    std::pair<std::string, std::deque<std::string>> match_string_with_var(std::string sentence);

    std::vector<std::string> match_batch(std::vector<std::string> sentences, int n_workers);

    std::pair<std::vector<std::string>, std::vector<std::deque<std::string>>> match_batch_with_var(
        std::vector<std::string> sentences, int n_workers
    );

    bool isEqual(Tree* other_tree);

    bool isEqual(MatchTree* matcher);

    void free_memory();

};
#endif 