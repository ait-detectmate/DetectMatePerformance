#ifndef M_PARSEDELEM_H
#define M_PARSEDELEM_H

#include <string>
#include <vector>
#include <utility>
#include "../aux.h"
#include <regex>




std::string postProcessTemp(const std::string& input);

std::vector<std::string> postProcessVars(std::vector<std::string> input_vars);

class ParsedElement {

public:
    int event_id;
    std::string log_template;
    std::vector<std::string> variables;

    ParsedElement(
        int event_id, std::string log_template, std::vector<std::string> variables
    );
    ParsedElement(int event_id, std::string log_template);

    ~ParsedElement();

};

#endif
