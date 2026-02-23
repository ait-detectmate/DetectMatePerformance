
#include "parsed.h"


ParsedMessages::ParsedMessages(Templates& templates) {
    std::deque<std::string> template_ = templates.getNextTemplate();
    int i = 0;
    while (!template_.empty()) {
        event_ids_map[template_] = i;
    }
}

ParsedMessages::~ParsedMessages() {
    this->variables.clear();
    this->event_ids.clear();
    this->event_ids_map.clear();
}

int ParsedMessages::getNext() {
    return 0;
}

void ParsedMessages::setNext(std::deque<std::string> template_) {
    // Implementation here
}

int ParsedMessages::size() {
    
    return 0;
}

std::pair<int, int> ParsedMessages::shape() {
    
    return {0, 0};
}
