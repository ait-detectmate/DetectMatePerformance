#include <gtest/gtest.h>

#include "../../src/detectmateperformance/_core/template_matcher/variables.h"


TEST(VariableTest, VariableClass) {
    Variables* variables1 = new Variables();
    variables1->add_variable("hello");
    variables1->add_variable("there");

    Variables* variables2 = new Variables();
    variables2->add_variable("General kenobi!");

    Variables* variables3 = new Variables();

    auto results = variables1->export_variables();
    auto results2 = variables2->export_variables();
    auto results3 = variables3->export_variables();

    std::deque<std::string> expected1 = {"hello", "there"};
    EXPECT_EQ(results, expected1);

    std::deque<std::string> expected2 = {"General kenobi!"};
    EXPECT_EQ(results2, expected2);

    std::deque<std::string> expected3 = {};
    EXPECT_EQ(results3, expected3);

    delete variables1;
    delete variables2;
    delete variables3;
}

TEST(VariableTest, VariableNotCapture) {
    Variables* variables1 = new Variables(false);
    variables1->add_variable("hello");
    variables1->add_variable("there");

    Variables* variables2 = new Variables(true);
    variables2->add_variable("General kenobi!");

    auto results = variables1->export_variables();
    auto results2 = variables2->export_variables();

    std::deque<std::string> expected1 = {};
    EXPECT_EQ(results, expected1);

    std::deque<std::string> expected2 = {"General kenobi!"};
    EXPECT_EQ(results2, expected2);

    delete variables1;
    delete variables2;
}
