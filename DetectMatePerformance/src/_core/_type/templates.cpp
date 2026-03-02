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

Templates::Templates(std::deque<std::string> templates) : Messages(templates) {
    for (auto& message : this->messages) {
        message = tpreprocess(message);
    }
}

Templates::Templates(std::string message) : Messages(message) {
}

Templates::~Templates() {
}

std::string Templates::getNextConcatenate() {
    std::vector<std::string> msg = Messages::getNext();
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
    return Messages::size();
}

std::pair<int, int> Templates::shape() {
    return Messages::shape();
}