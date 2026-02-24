#include "tree.h"
#include <algorithm>  
#include <utility> 
#include <string>


const std::string VARIABLE_SYMBOL = "VAR";


Tree::Tree(std::string value){
    data = value;
}

Tree::Tree(std::string value, std::string template_) {
    data = value;
    full_template = template_;
}

Tree::~Tree() {
    for (Tree* child : getChildren()) {
        delete child;  
    }
}

void Tree::addChild(Tree* child) {
    children.push_back(child);
}

const std::vector<Tree*>& Tree::getChildren() const {
    return children;
}

std::string Tree::getData() const {
    return data;
}

std::string Tree::getTemplate() const {
    return full_template;
}

void Tree::setTemplate(std::string value) {
    full_template = value;
}

bool Tree::isFullTemplate() const{
    return !full_template.empty();
}

std::pair<bool, Tree*> Tree::contains(std::string value) const {
    for (Tree* child: getChildren()) {
        if (child->getData() == value) {
            return std::make_pair(true, child);
        }
    }
    return std::make_pair(false, nullptr);
}

std::pair<bool, Tree*> Tree::contains_variable() const {
    return contains(VARIABLE_SYMBOL);
}

bool Tree::isEqual(Tree* node) {

    if (getData() != node->getData() || isFullTemplate() != node->isFullTemplate()) {
        return false;
    }

    if (getChildren().size() != node->getChildren().size()) {
        return false;
    }

    std::pair<bool, Tree*> result;      
    for (Tree* child: getChildren()) {
        result = node->contains(child->getData());

        if (result.first) {
            if (!child->isEqual(result.second)) {
                return false;
            }             
        } else {
            return false;
        }

    }

    return true;
}