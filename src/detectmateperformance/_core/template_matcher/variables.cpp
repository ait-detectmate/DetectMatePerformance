
#include "variables.h"

#include <vector>
#include <utility>
#include <string>



Variables::Variables() {
    this->capture_vars = true;
    this->list = "";
}

Variables::Variables(bool get_variables) {
    this->capture_vars = get_variables;
}

Variables::~Variables() {
    this->list.clear();
}

void Variables::add_variable(std::string variable) {
    if (capture_vars) {
        if (this->list.size() > 0)
            this->list.append(" ");
        this->list.append(variable);
    }
}

void Variables::add_variables(std::deque<std::string> variables) {
    if (capture_vars) {
        for (std::string variable : variables) {
            this->add_variable(variable);
         }
    }
}

void Variables::init_list() {
    this->list.clear();
}

std::string Variables::export_variables() const {
    return this->list;
}
