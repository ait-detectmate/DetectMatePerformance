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
    std::vector<std::string> variables;
    std::vector<int> event_ids;

    std::unordered_map<std::string, int> event_ids_map;
    std::vector<std::string> id_to_template;

public:
    ParsedMessages(Templates* templates, int n);

    ~ParsedMessages();

    int getElemID(int n);
    std::string getElem(int n);
    std::pair<std::string, std::string> getElemWithVar(int n);
    void setElem(int n, std::string template_);
    void setElemWithVar(
        int n, std::string template_, std::string vars
    );

    std::vector<int> getAllIDs();
    std::vector<std::string> getAllElemts();
    std::vector<std::string> getAllVar();

    int size();
    std::pair<int, int> shape();

};
#endif
