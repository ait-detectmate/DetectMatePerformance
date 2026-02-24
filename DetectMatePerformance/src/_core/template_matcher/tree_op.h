#ifndef TREE_OP_H
#define TREE_OP_H

#include "tree.h"
#include "variables.h"

#include <vector>
#include <string>
#include <utility>
#include <deque>


std::pair<bool, Tree*> searchTree(Tree* root, std::deque<std::string>& sequence, Variables* variables);

void addSequence(Tree* root, std::deque<std::string>& sequence, const std::string& full_template);

Tree* buildTree(std::deque<std::string>& sequences);

std::deque<std::string> preprocess(std::string& sentence);

#endif