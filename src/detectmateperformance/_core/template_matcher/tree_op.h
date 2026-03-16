#ifndef TREE_OP_H
#define TREE_OP_H

#include "tree.h"
#include "variables.h"

#include <vector>
#include <string>
#include <utility>
#include <deque>

#include "../_type/templates.h"

std::pair<bool, Tree*> searchTree(Tree* root, std::deque<std::string>& sequence, Variables* variables);

void addSequence(
    Tree* root, std::vector<std::string>& sequence, const std::string& full_template, int i
);

Tree* buildTree(Templates* sequences);


#endif
