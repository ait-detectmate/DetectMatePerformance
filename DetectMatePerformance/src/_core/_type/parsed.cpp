
#include "parsed.h"

ParsedMessages::ParsedMessages(Templates* templates) {
    templates->resetCount();
    std::string template_ = templates->getNextConcatenate();

    int i = 0;
    while (template_ != " ") {
        this->event_ids_map[template_] = i;
        this->id_to_template.push_back(template_);
        template_ = templates->getNextConcatenate();
        i++;
    }
}

ParsedMessages::~ParsedMessages() {
    this->variables.clear();
    this->event_ids.clear();
    this->event_ids_map.clear();
    this->id_to_template.clear();
}

std::string ParsedMessages::getNext() {
    if (event_ids.empty() || this->count >= event_ids.size()) {
        return "";
    }
    
    int event_idsf = event_ids[this->count];
    this->count++;
    
    if (event_idsf == -1) {
        return "template not found";
    }
 
    return id_to_template[event_idsf];
}

void ParsedMessages::setNext(std::string template_) {
    if (event_ids_map.find(template_) == event_ids_map.end()){
        event_ids.push_back(-1);
    } else {
        event_ids.push_back(event_ids_map[template_]);
    }
}

int ParsedMessages::size() {
    
    return event_ids.size();
}

std::pair<int, int> ParsedMessages::shape() {
    if (event_ids.empty()) {
        return {0, 0};
    }

    int max_length = 0;
    for (const auto& vars : this->variables) {
        max_length = std::max(max_length, static_cast<int>(vars.size()));
    }
    
    return {event_ids.size(), max_length};
}

void ParsedMessages::resetCount() {
    count = 0;
}