#ifndef M_PARSED_H
#define M_PARSED_H

#include <utility> 
#include <string>
#include <unordered_map>
#include <deque>

#include "templates.h"

#include <functional> 


struct ListHasher {
    size_t operator()(const std::deque<std::string>& list) const {
        size_t seed = 0;
        for (const auto& str : list) {
            // Combine the hash o' each string with the seed
            seed ^= std::hash<std::string>{}(str) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};


class ParsedMessages {
protected:
    std::deque<std::deque<std::string>> variables;
    std::deque<int> event_ids;
    std::unordered_map<std::deque<std::string>, int, ListHasher> event_ids_map;
 
public:
    ParsedMessages(Templates& templates);

    ~ParsedMessages();

    int getNext();
    void setNext(std::deque<std::string> template_);

    int size();
    std::pair<int, int> shape();
};
#endif 