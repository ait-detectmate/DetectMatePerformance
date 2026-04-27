#ifndef M_PARSEDELEM_H
#define M_PARSEDELEM_H

#include <string>
#include <deque>
#include "../aux.h"
#include <regex>




std::string postProcessTemp(const std::string& input);

std::deque<std::string> postProcessVars(const std::string& input_vars);

class ParsedElement {

public:
    int event_id;
    std::string log_template;
    std::deque<std::string> variables;

    ParsedElement(
        int event_id, std::string log_template, std::string variables
    );
    ParsedElement(int event_id, std::string log_template);

    ~ParsedElement();

};

#endif
