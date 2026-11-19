#include <gtest/gtest.h>

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../src/detectmateperformance/_core/parsers/brain.h"

TEST(BrainTest, ReconstructsWordsInPositionOrder) {
    const std::vector<std::string> expected = {"hello there"};
    EXPECT_EQ(brain({2}, {{{1, "there"}, {0, "hello"}}}), expected);
}

TEST(BrainTest, FillsMissingPositions) {
    const std::vector<std::string> expected = {
        "<*> user <*> login <*> <*>",
        "hello there <*>",
    };

    EXPECT_EQ(
        brain({6, 3}, {{{3, "login"}, {1, "user"}}, {{0, "hello"}, {1, "there"}}}),
        expected
    );
}

TEST(BrainTest, HandlesEmptyInput) {
    const std::vector<std::string> expected = {"<*> <*>", ""};
    EXPECT_EQ(brain({2, 0}, {{}, {}}), expected);
    EXPECT_TRUE(brain({}, {}).empty());
}

TEST(BrainTest, RejectsMismatchedRowCounts) {
    EXPECT_THROW(brain({1}, {}), std::invalid_argument);
}

TEST(BrainTest, RejectsInvalidLengths) {
    EXPECT_THROW(brain({-1}, {{}}), std::invalid_argument);
    EXPECT_THROW(brain({std::nullopt}, {{}}), std::invalid_argument);
}

TEST(BrainTest, RejectsOutOfRangePositions) {
    EXPECT_THROW(brain({2}, {{{-1, "word"}}}), std::invalid_argument);
    EXPECT_THROW(brain({2}, {{{2, "word"}}}), std::invalid_argument);
}
