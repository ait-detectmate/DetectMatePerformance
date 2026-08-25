
#include "element.h"


std::string postProcessTemp(const std::string& input){
    return input;  // For now it will be save like this
}

std::vector<std::string> postProcessVars(std::vector<std::string> input_vars) {
    return input_vars;  // For now it will be save like this
}

ParsedElement::ParsedElement(
    int event_id, std::string log_template, std::vector<std::string> variables
) {
    this->event_id = event_id;
    this->log_template = postProcessTemp(log_template);
    this->variables = postProcessVars(std::move(variables));
}

ParsedElement::ParsedElement(
    int event_id, std::string log_template
) {
    this->event_id = event_id;
    this->log_template = postProcessTemp(log_template);
    this->variables = {};
}

ParsedElement::~ParsedElement() {

}
