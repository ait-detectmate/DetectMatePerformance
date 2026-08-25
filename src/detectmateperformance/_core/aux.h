#ifndef M_AUX_H
#define M_AUX_H

#include <vector>
#include <utility>
#include <string>
#include <deque>
#include <algorithm>

struct Token {
    std::string word;
    size_t begin;  // byte offset of the first char in the original message
    size_t end;    // one past the last char
};

std::deque<Token> tokenize(const std::string& message);

std::deque<std::string> preprocessing(std::string message);

#endif
