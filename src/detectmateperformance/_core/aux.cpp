#include "aux.h"

#include <cctype>

std::deque<Token> tokenize(const std::string& message) {
    // Same splitting rule as the old preprocessing(): every punctuation char acts as
    // a separator (it used to be overwritten with ' ' in place, then split on ' ').
    // std::ispunct receives the raw (possibly signed) char exactly like before, so
    // byte-level behavior -- including for non-ASCII bytes -- is identical.
    // Also mirrors the old C-string scan: stops at the first embedded NUL byte,
    // dropping everything after it (like `while (*end)` did).
    std::deque<Token> tokens;
    size_t start = 0;
    for (size_t i = 0; i <= message.size(); ++i) {
        // Stop scanning at first NUL, mirroring the old C-string walk
        if (i < message.size() && message[i] == '\0') {
            if (i > start) {
                tokens.push_back({message.substr(start, i - start), start, i});
            }
            return tokens;
        }
        bool sep = i == message.size() || message[i] == ' ' || std::ispunct(message[i]);
        if (sep) {
            if (i > start) {
                tokens.push_back({message.substr(start, i - start), start, i});
            }
            start = i + 1;
        }
    }
    return tokens;
}

std::deque<std::string> preprocessing(std::string message) {
    std::deque<std::string> words;
    for (const Token& token : tokenize(message)) {
        words.push_back(token.word);
    }
    return words;
}
