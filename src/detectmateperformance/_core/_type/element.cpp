
#include "element.h"
#include "../aux.h"
#include <regex>


std::string postProcessTemp(const std::string& input){
    std::string pattern = "\\bVAR\\b";
    std::regex re(pattern);
    return std::regex_replace(input, re, "<*>");
}

ParsedElement::ParsedElement(
    int event_id, std::string log_template, std::string variables
) {
    this->event_id = event_id;
    this->log_template = postProcessTemp(log_template);
    this->variables = preprocessing(variables);
}

ParsedElement::~ParsedElement() {

}
