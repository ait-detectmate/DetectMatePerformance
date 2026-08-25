#include "tree_op.h"
#include "tree.h"
#include "variables.h"

#include <algorithm>
#include <sstream>
#include <deque>

std::pair<bool, Tree*> __searchTree(
    Tree* node, std::deque<Token>& sequence, Variables* variables, bool in_var
) {
    // in_var: it is inside the special variable

    // Sequence is empty but the tree is not
    if (sequence.empty()) {
        return std::make_pair(false, nullptr);
    }

    Token head = sequence.front();
    std::pair<bool, Tree*> result = node->contains(head.word);

    // Next element in the sequence was found in the tree
    if (result.first && !in_var){
        // Sequence only have one more element
        if (sequence.size() == 1) {
            return std::make_pair(result.second->isFullTemplate(), result.second);
        } else {
            sequence.pop_front();
            return __searchTree(result.second, sequence, variables, false);
        }
    }

    // sequence element not found but the tree has <*>
    result = node->contains_variable();
    if (result.first) {
        // The tree finish in <*> and that is ok because is the last element:
        // one slot covering the rest of the line, verbatim
        if (result.second->getChildren().size() == 0 && result.second->isFullTemplate()) {
            variables->extend_span(sequence.front(), sequence.back());
            variables->close();
            return std::make_pair(result.second->isFullTemplate(), result.second);
        }

        std::pair<bool, Tree*> sub_result = result.second->contains(head.word);
        // The next element after <*> is found: finalize the wildcard's slot
        // ("" if it matched zero tokens)
        if (sub_result.first) {
            variables->close_or_empty();
            if (sequence.size() == 1) {
                return std::make_pair(sub_result.second->isFullTemplate(), sub_result.second);
            } else {
                sequence.pop_front();
                return __searchTree(sub_result.second, sequence, variables, false);
            }
        } else {
        // No more elements in the sequence: the last token belongs to the wildcard
            if (sequence.size() == 1) {
                variables->extend(head);
                variables->close();
                return std::make_pair(result.second->isFullTemplate(), result.second);
            }
            // The next element after <*> was not found, keep going deeper in the sequence
            variables->extend(head);
            sequence.pop_front();
            return __searchTree(node, sequence, variables, true);
        }
    }

    return std::make_pair(false, nullptr);
}

std::pair<bool, Tree*> searchTree(
    Tree* node, std::deque<Token>& sequence, Variables* variables
) {
    return __searchTree(node, sequence, variables, false);
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

            if (sequence.size() - 1 == i) {
                child->setTemplate(template_);
            }

        }
    }

    i = i + 1;
    if (i < sequence.size()) {
        addSequence(child, sequence, template_, i);
    }

}

Tree* buildTree(Templates* sequences) {
    Tree* root = new Tree("");

    std::vector<std::string> temp = sequences->getNext();
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
