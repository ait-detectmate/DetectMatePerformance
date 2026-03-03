#include "aux.h"


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