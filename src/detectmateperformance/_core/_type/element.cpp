
#include "element.h"


std::string postProcessTemp(const std::string& input){
    std::string pattern = "\\bVAR\\b";
    std::regex re(pattern);
    std::string result = std::regex_replace(input, re, "<*>");
    return result;
}

std::deque<std::string> postProcessVars(const std::string& input_vars) {
    return preprocessing(input_vars);
}

ParsedElement::ParsedElement(
    int event_id, std::string log_template, std::string variables
) {
    this->event_id = event_id;
    this->log_template = postProcessTemp(log_template);
    this->variables = postProcessVars(variables);
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
