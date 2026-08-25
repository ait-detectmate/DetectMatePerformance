#include <gtest/gtest.h>

#include <vector>
#include <string>

#include "../../src/detectmateperformance/_core/template_matcher/match_tree.h"

static std::vector<std::string> capture(const std::string& tmpl, const std::string& line) {
    Templates templates(tmpl);            // template uses VAR for wildcards
    MatchTree matcher(&templates);
    ParsedMessages* result = matcher.match_string_with_var(line);
    std::vector<std::string> vars = result->getElemWithVar(0).variables;
    delete result;
    return vars;
}

TEST(VerbatimCapture, HdfsCompoundsStayIntact) {
    std::vector<std::string> vars = capture(
        "Receiving block VAR src: VAR dest: VAR",
        "Receiving block blk_-1608999687919862906 src: /10.250.19.102:54106 dest: /10.250.19.102:50010");
    ASSERT_EQ(vars.size(), 3u);
    EXPECT_EQ(vars[0], "blk_-1608999687919862906");  // sign survives (interior punctuation)
    // leading '/' is separator punctuation adjacent to the slot -> not captured (documented limitation)
    EXPECT_EQ(vars[1], "10.250.19.102:54106");
    EXPECT_EQ(vars[2], "10.250.19.102:50010");
}

TEST(VerbatimCapture, ZeroWidthWildcardKeepsSlot) {
    std::vector<std::string> vars = capture("alpha VAR beta VAR gamma", "alpha beta x gamma");
    ASSERT_EQ(vars.size(), 2u);
    EXPECT_EQ(vars[0], "");
    EXPECT_EQ(vars[1], "x");
}

TEST(VerbatimCapture, AdjacentWildcardsOneCombinedSlot) {
    // buildTree collapses VAR VAR to one node; the stored template has one wildcard
    std::vector<std::string> vars = capture("alpha VAR VAR beta", "alpha x y beta");
    ASSERT_EQ(vars.size(), 1u);
    EXPECT_EQ(vars[0], "x y");
}

TEST(VerbatimCapture, TailWildcardVerbatim) {
    std::vector<std::string> vars = capture("cmd VAR", "cmd rm -rf /tmp/x, now");
    ASSERT_EQ(vars.size(), 1u);
    EXPECT_EQ(vars[0], "rm -rf /tmp/x, now");
}

TEST(VerbatimCapture, UnmatchedLineBehaviorUnchanged) {
    Templates templates(std::string("alpha VAR beta"));
    MatchTree matcher(&templates);
    ParsedMessages* result = matcher.match_string_with_var("something else entirely");
    EXPECT_EQ(result->getElemWithVar(0).log_template, "template not found");
    delete result;

    // partial capture on an unmatched line: the walk enters the wildcard, consumes to
    // the end and never finds the closing literal -- no template, but the still-open
    // slot is exported verbatim ("template not found" rows are filtered downstream)
    Templates templates2(std::string("a VAR c"));
    MatchTree matcher2(&templates2);
    ParsedMessages* result2 = matcher2.match_string_with_var("a x y z");
    ParsedElement elem2 = result2->getElemWithVar(0);
    EXPECT_EQ(elem2.log_template, "template not found");
    std::vector<std::string> expected2 = {"x y z"};
    EXPECT_EQ(elem2.variables, expected2);
    delete result2;
}
