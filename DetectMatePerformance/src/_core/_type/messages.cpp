#include <algorithm>
#include "message.h"

#include "../aux.h"


Messages::Messages(std::deque<std::string> messages) {
    for (const auto& message : messages) {
        this->messages.push_back(preprocessing(message));
    }
}

Messages::Messages(std::string message) {
    this->messages.push_back(preprocessing(message));
}

Messages::~Messages() {
    this->messages.clear();
}

std::deque<std::string> Messages::getNext() {
    if (this->messages.empty()) {
        return {};
    }
    auto message = this->messages.front();
    this->messages.pop_front();

    return message;
}

std::string Messages::getNextConcatenate() {
    std::deque<std::string> msg = Messages::getNext();

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

int Messages::size() {
    return this->messages.size();
}

std::pair<int, int> Messages::shape() {
    if (this->messages.empty()) {
        return {0, 0};
    }
    int max_length = 0;
    for (const auto& message : this->messages) {
        max_length = std::max(max_length, static_cast<int>(message.size()));
    }

    return {this->messages.size(), max_length};
}