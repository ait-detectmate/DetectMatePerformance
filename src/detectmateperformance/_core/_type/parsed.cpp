
#include "parsed.h"

ParsedMessages::ParsedMessages(Templates* templates, int n) {
    templates->resetCount();
    std::string template_ = templates->getNextConcatenate();

    this->event_ids.resize(n);
    this->variables.resize(n);

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


int ParsedMessages::getElemID(int n) {
    return this->event_ids[n];
}

std::string ParsedMessages::getElem(int n) {

    int event_idsf = this->getElemID(n);
    if (event_idsf == -1) {
        return "template not found";
    }

    return id_to_template[event_idsf];
}

std::pair<std::string, std::string> ParsedMessages::getElemWithVar(int n) {
    std::string temp = ParsedMessages::getElem(n);
    std::string vars = this->variables[n];

    return std::make_pair(temp, vars);
}

void ParsedMessages::setElem(int n, std::string template_) {
    if (event_ids_map.find(template_) == event_ids_map.end()){
        this->event_ids[n] = -1;
    } else {
        this->event_ids[n] = event_ids_map[template_];
    }
}

void ParsedMessages::setElemWithVar(
    int n, std::string template_, std::string vars
) {

    ParsedMessages::setElem(n, template_);
    this->variables[n] = vars;

}

std::vector<int> ParsedMessages::getAllIDs() {
    std::vector<int> event_ids(this->size());

    for (int i = 0; i < this->size(); i++) {
        event_ids[i] = this->getElemID(i);
    }

    return event_ids;
}

std::vector<std::string> ParsedMessages::getAllElemts() {
    std::vector<std::string> templates(this->size());

    for (int i = 0; i < this->size(); i++) {
        templates[i] = this->getElem(i);
    }

    return templates;
}

std::vector<std::string> ParsedMessages::getAllVar() {
    return this->variables;
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
