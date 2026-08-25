#include <gtest/gtest.h>

#include "../../src/detectmateperformance/_core/template_matcher/variables.h"

static Token tok(const std::string& s, size_t begin) {
    return Token{s, begin, begin + s.size()};
}

TEST(VariableTest, SlotsSliceVerbatim) {
    std::string line = "blk_-42 rest";
    Variables vars;
    vars.extend(tok("blk", 0));
    vars.extend(tok("42", 5));   // grows the open slot across the "_-"
    vars.close();
    vars.extend(tok("rest", 8));
    vars.close();

    std::vector<std::string> expected = {"blk_-42", "rest"};
    EXPECT_EQ(vars.export_variables(line), expected);
}

TEST(VariableTest, CloseOrEmptyRecordsZeroWidthSlot) {
    Variables vars;
    vars.close_or_empty();               // wildcard matched zero tokens
    vars.extend(tok("x", 2));
    vars.close_or_empty();               // open slot -> behaves like close()
    std::vector<std::string> expected = {"", "x"};
    EXPECT_EQ(vars.export_variables("a x"), expected);
}

TEST(VariableTest, ExtendSpanCoversRestOfLine) {
    std::string line = "cmd rm -rf /tmp/x";
    Variables vars;
    vars.extend_span(tok("rm", 4), tok("x", 16));
    vars.close();
    std::vector<std::string> expected = {"rm -rf /tmp/x"};
    EXPECT_EQ(vars.export_variables(line), expected);
}

TEST(VariableTest, InitListResets) {
    Variables vars;
    vars.extend(tok("x", 0));
    vars.close();
    vars.init_list();
    EXPECT_TRUE(vars.export_variables("x").empty());
}

TEST(VariableTest, NotCaptureIsNoOp) {
    Variables vars(false);
    vars.extend(tok("x", 0));
    vars.close_or_empty();
    EXPECT_TRUE(vars.export_variables("x").empty());
}
