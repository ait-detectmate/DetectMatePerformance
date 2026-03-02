#include <algorithm>
#include "message.h"


bool do_split(const char* str) {
    return *str == ' ';
}

void remove_empty(std::vector<std::string>& words) {
    words.erase(std::remove_if(words.begin(), words.end(), [](const std::string& word) {
        return word.empty();
    }), words.end());
}

std::vector<std::string> preprocessing(std::string message) {
    std::vector<std::string> words;
    
    const char* start = message.data();  
    const char* end = start;

    while (*end) {
        if (std::ispunct(*end)) {
            *const_cast<char*>(end) = ' ';
        }

        if (do_split(end)) {
            words.emplace_back(start, end);
            start = end + 1;  
        }
        end++;

    }
    words.emplace_back(start, end);
    remove_empty(words);

    return words;
}

Messages::Messages(std::deque<std::string> messages) {
    for (const auto& message : messages) {
        this->messages.push_back(preprocessing(message));
    }
    resetCount();
}

Messages::Messages(std::string message) {
    this->messages.push_back(preprocessing(message));
    resetCount();
}

Messages::~Messages() {
    this->messages.clear();
}

std::vector<std::string> Messages::getNext() {
    if (this->messages.empty()) {
        return {};
    }
    auto message = this->messages[this->count];
    this->count++;

    return message;
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

void Messages::resetCount() {
    this->count = 0;
}