#include "tree_op.h"
#include "tree.h"
#include "variables.h"

#include <algorithm>
#include <sstream>
#include <deque>


std::pair<bool, Tree*> searchTree(
    Tree* node, std::deque<std::string>& sequence, Variables* variables
) {

    // Sequence is empty but the tree is not
    if (sequence.empty()) {
        return std::make_pair(false, nullptr);
    }

    std::string head = sequence.front();
    std::pair<bool, Tree*> result = node->contains(head);

    // Next element in the sequence was found in the tree
    if (result.first){
        // Sequence only have one more element
        if (sequence.size() == 1) {
            return std::make_pair(result.second->isFullTemplate(), result.second);
        } else {
            sequence.pop_front();
            return searchTree(result.second, sequence, variables);
        }

    }

    // sequence element not found but the tree has <*>
    result = node->contains_variable();
    if (result.first) {
        // The tree finish in <*> and that is ok because is the last element
        if (result.second->getChildren().size() == 0 && result.second->isFullTemplate()) {
            variables->add_variables(sequence);
            return std::make_pair(result.second->isFullTemplate(), result.second);
        }

        std::pair<bool, Tree*> sub_result = result.second->contains(head);
        // The next element after <*> is found
        if (sub_result.first) {
            if (sequence.size() == 1) {
                return std::make_pair(sub_result.second->isFullTemplate(), sub_result.second);
            } else {
                sequence.pop_front();
                return searchTree(sub_result.second, sequence, variables);
            }
        } else {
            // The next element after <*> was not found, keep going deeper in the sequence
            variables->add_variable(head);
            sequence.pop_front();
            return searchTree(node, sequence, variables);
        }

    }

    return std::make_pair(false, nullptr);
}

void addSequence(
    Tree* node, std::vector<std::string>& sequence, const std::string& template_, int i
) {

    std::string head = sequence[i];
    Tree* child;

    if (head == VARIABLE_SYMBOL && node->getData() == VARIABLE_SYMBOL) {
        child = node;
        // In case the last element are two or more <*>
        if (sequence.size() - 1 == i) {
            child->setTemplate(template_);
        }

    } else {
        std::pair<bool, Tree*> result = node->contains(head);
        if (!result.first) {

            if (sequence.size() - 1 == i) {
                child = new Tree(head, template_);
            } else {
                child = new Tree(head);
            }
            node->addChild(child);

        } else {

            child = result.second;

        }
    }

    i = i + 1;
    if (i < sequence.size()) {
        addSequence(child, sequence, template_, i);
    }

}

Tree* buildTree(Templates* sequences) {
    Tree* root = new Tree("");

    auto temp = sequences->getNext();
    std::string full_temp;
    while (temp.size() > 0) {
        full_temp = "";
        for (size_t i = 0; i < temp.size(); i++){
            if (i != 0) {
                full_temp += " ";
            }
            full_temp += temp[i];
        }
        addSequence(root, temp, full_temp, 0);
        temp = sequences->getNext();
    }

    return root;
}
