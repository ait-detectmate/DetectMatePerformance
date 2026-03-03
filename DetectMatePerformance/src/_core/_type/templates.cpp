#include <vector>
#include <utility> 
#include <string>
#include <deque>
#include "templates.h"


std::vector<std::string> tpreprocess(std::vector<std::string> message) {
    // remove multiple VARs in a row
    std::vector<std::string> processed;
    for (const auto& word : message) {
        if (word == "VAR") {
            if (!processed.empty() && processed.back() == "VAR") {
                continue; // skip this VAR
            }
        }
        processed.push_back(word);
    }

    return processed;
}

Templates::Templates(std::deque<std::string> templates) {
    for (const auto& message : templates) {
        this->messages.push_back(tpreprocess(preprocessing(message)));
    }
    resetCount();
}

Templates::Templates(std::string message) {
    this->messages.push_back(preprocessing(message));
    resetCount();
}

Templates::~Templates() {
    this->messages.clear();
}

std::vector<std::string> Templates::getNext() {
    if (this->messages.empty()) {
        return {};
    }
    auto message = this->messages[this->count];
    this->count++;

    return message;
}

std::string Templates::getNextConcatenate() {
    std::vector<std::string> msg = Templates::getNext();

    if (msg.size() == 0)
        return " ";

    std::string result;
     for (size_t i = 0; i < msg.size(); ++i) {
        if (i != 0) {
            result += " ";
        }
        result += msg[i];
    }

    return result;
}

int Templates::size() {
    return this->messages.size();
}

std::pair<int, int> Templates::shape() {
    if (this->messages.empty()) {
        return {0, 0};
    }
    int max_length = 0;
    for (const auto& message : this->messages) {
        max_length = std::max(max_length, static_cast<int>(message.size()));
    }

    return {this->messages.size(), max_length};
}

void Templates::resetCount() {
    this->count = 0;
}