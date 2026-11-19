#ifndef BRAIN_H_
#define BRAIN_H_

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using PositionedWord = std::pair<std::int64_t, std::string>;

std::vector<std::string> brain(
    const std::vector<std::optional<std::int64_t>>& lengths,
    const std::vector<std::vector<PositionedWord>>& templates
);

#endif  // BRAIN_H_
