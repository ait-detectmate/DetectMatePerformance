#ifndef M_PARSEDELEM_H
#define M_PARSEDELEM_H

#include <string>
#include <deque>


class ParsedElement {

public:
    int event_id;
    std::string log_template;
    std::deque<std::string> variables;

    ParsedElement(
        int event_id, std::string log_template, std::string variables
    );

    ~ParsedElement();

};

#endif
