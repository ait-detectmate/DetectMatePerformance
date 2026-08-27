#include <regex>

#include "aux.h"


std::string clean_string(const std::string& input) {
    std::regex pattern(
        "[!\"#$%&'()*+,/:;<=>?@\\[\\]^`{|}~\\s]|\\.(?![a-zA-Z0-9])|-(?![a-zA-Z0-9])"
    );
    std::string result = std::regex_replace(input, pattern, " ");

    return result;
}

bool do_split(const char* str) {
    return *str == ' ';
}

void remove_empty(std::deque<std::string>& words) {
    words.erase(std::remove_if(words.begin(), words.end(), [](const std::string& word) {
        return word.empty();
    }), words.end());
}

std::deque<std::string> preprocessing(std::string message) {
    std::deque<std::string> words;

    message = clean_string(message);
    const char* start = message.data();
    const char* end = start;

    while (*end) {
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
