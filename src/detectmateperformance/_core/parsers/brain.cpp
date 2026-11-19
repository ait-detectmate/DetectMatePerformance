#include "brain.h"

#include <cstddef>
#include <stdexcept>
#include <string_view>


bool doSizesMatch(
    const std::vector<std::optional<std::int64_t>>& lengths,
    const std::vector<std::vector<PositionedWord>>& templates
) {
    return lengths.size() != templates.size();
}

bool isLengthNegative(
    const std::vector<std::optional<std::int64_t>>& lengths, std::size_t row
) {
    return !lengths[row] || *lengths[row] < 0;
}

bool isPositionInRange(int length, int position) {
    return position < 0 || position >= length;
}

void concatenateString(
    std::vector<std::string_view> tokens, std::string& sentence
) {
    sentence.clear();
    for (std::size_t index = 0; index < tokens.size(); ++index) {
        if (index > 0) {
            sentence += ' ';
        }
        sentence += tokens[index];
    }
}

std::vector<std::string> brain(
    const std::vector<std::optional<std::int64_t>>& lengths,
    const std::vector<std::vector<PositionedWord>>& templates
) {
    if (doSizesMatch(lengths, templates)) {
        throw std::invalid_argument("Lenght and Template must have the same number of rows");
    }

    std::vector<std::string> sentences;
    std::vector<std::string_view> tokens;
    std::string sentence;
    std::int64_t length;
    std::int64_t position;

    sentences.reserve(lengths.size());

    for (std::size_t row = 0; row < lengths.size(); ++row) {
        if (isLengthNegative(lengths, row)) {
            throw std::invalid_argument("Lenght must be a nonnegative integer");
        }

        length = *lengths[row];
        tokens.assign(static_cast<std::size_t>(length), "<*>");
        for (std::size_t index = 0; index < templates[row].size(); ++index) {

            position = templates[row][index].first;
            if (isPositionInRange(length, position)) {
                throw std::invalid_argument("Template position is outside Lenght");
            }

            tokens[static_cast<std::size_t>(position)] = templates[row][index].second;
        }

        concatenateString(tokens, sentence);
        sentences.push_back(std::move(sentence));
    }

    return sentences;
}
