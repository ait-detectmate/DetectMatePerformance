
#include "element.h"

ParsedElement::ParsedElement(
    int event_id, std::string log_template, std::string variables
) {
    this->event_id = event_id;
    this->log_template = log_template;
    this->variables = variables;
}

ParsedElement::~ParsedElement() {

}
