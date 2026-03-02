#ifndef M_PARSED_H
#define M_PARSED_H

#include <utility> 
#include <string>
#include <unordered_map>
#include <deque>

#include "templates.h"

#include <functional> 


class ParsedMessages {
protected:
    std::deque<std::deque<std::string>> variables;
    std::vector<int> event_ids;

    std::unordered_map<std::string, int> event_ids_map;
    std::vector<std::string> id_to_template;
    int count;
 
public:
    ParsedMessages(Templates* templates);

    ~ParsedMessages();

    std::string getNext();
    void setNext(std::string template_);

    int size();
    std::pair<int, int> shape();

    void resetCount();
};
#endif 