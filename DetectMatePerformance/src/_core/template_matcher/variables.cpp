
#include "variables.h"

#include <vector>
#include <utility> 
#include <string>



Variables::Variables() {
    capture_vars = true;
}
    
Variables::Variables(bool get_variables) {
    capture_vars = get_variables;
}

Variables::~Variables() {
    list.clear();
}

void Variables::add_variable(std::string variable) {
    if (capture_vars) {
        list.push_back(variable);
    }
}

void Variables::add_variables(std::deque<std::string> variables) {
    if (capture_vars) {
        for (std::string variable : variables) {
            add_variable(variable);
         }
    }
}

void Variables::init_list() {
    list.clear();
}

std::deque<std::string> Variables::export_variables() const {
    return list;
}
