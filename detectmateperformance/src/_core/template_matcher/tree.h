#ifndef TREE_H  
#define TREE_H

#include <vector>
#include <utility> 
#include <string>


extern const std::string VARIABLE_SYMBOL;


class Tree {
private:
    std::string data;     
    std::string full_template = "";

    std::vector<Tree*> children;  

public:
    Tree(std::string value);

    Tree(std::string value, std::string template_);

    ~Tree();

    void addChild(Tree* child);

    const std::vector<Tree*>& getChildren() const;

    std::string getData() const;

    std::string getTemplate() const;

    void setTemplate(std::string value);

    bool isFullTemplate() const;

    std::pair<bool, Tree*> contains(std::string value) const;

    std::pair<bool, Tree*> contains_variable() const;

    bool isEqual(Tree* node);
};

#endif