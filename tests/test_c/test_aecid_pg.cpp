#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

#include "../../src/detectmateperformance/_core/parsers/aecid_pg.h"

TEST(TreeOpTest, AddLog) {
    Tree tree = initTree();
    addLog(tree, {"hello", "there"});
    const Tree expected = Tree{
        {{"hello", Tree{{{"there", Tree{{}, {}, true}}}, {{"there", 1}}, false}}},
        {{"hello", 1}},
        false};

    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, AddLogs) {
    Tree tree = initTree();
    const Tree expected =
        Tree{{{"hello",
              Tree{{{"there", Tree{{}, {}, true}}, {"ciao", Tree{{}, {}, true}}},
                    {{"there", 1}, {"ciao", 1}},
                    false}}},
            {{"hello", 2}},
            false};

    const std::vector<std::string> logs = {"hello there", "hello ciao"};
    addLogs(tree, logs);
    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, MergeLeft) {
    Tree tree_1 =
        Tree{{{"hello",
              Tree{{{"there", Tree{{}, {}, true}}, {"ciao", Tree{{}, {}, true}}},
                    {{"there", 1}, {"ciao", 1}},
                    false}}},
            {{"hello", 2}},
            false};
    Tree tree_2 = Tree{
        {{"ciao", Tree{{}, {}, true}},
        {"hello", Tree{{{"ciao", Tree{{}, {}, true}}, {"a", Tree{{}, {}, true}}},
                        {{"ciao", 2}, {"a", 1}},
                        false}}},
        {{"ciao", 1}, {"hello", 1}},
        false};
    const Tree expected =
        Tree{{{"ciao", Tree{{}, {}, true}},
              {"hello", Tree{{{"there", Tree{{}, {}, true}},
                              {"ciao", Tree{{}, {}, true}},
                              {"a", Tree{{}, {}, true}}},
                            {{"there", 1}, {"ciao", 3}, {"a", 1}},
                            false}}},
            {{"ciao", 1}, {"hello", 3}},
            false};

    mergeLeft(tree_1, tree_2);
    EXPECT_EQ(tree_1, expected);
}

TEST(TreeOpTest, PruneWithRule1) {
    Tree tree = Tree{
        {{"hello",
          Tree{{{"a", Tree{{}, {}, true}},
                {"b", Tree{{}, {}, true}},
                {"c", Tree{{}, {}, true}},
                {"d", Tree{{{"ciao", Tree{{}, {}, true}}}, {{"ciao", 1}}, false}},
                {"e", Tree{{}, {}, true}}},
              {{"a", 2}, {"b", 2}, {"c", 2}, {"d", 2}, {"e", 1}},
              false}}},
        {{"hello", 4}},
        false};
    prune(tree, {0.2, 0.8, 1.0});
    const Tree expected =
        Tree{{{"hello",
              Tree{{{"<*>",
                      Tree{{{"ciao", Tree{{}, {}, true}}}, {{"ciao", 1}}, true}}},
                    {{"<*>", 9}},
                    false}}},
            {{"hello", 4}},
            false};

    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, PruneWithRule2) {

    Tree tree = Tree{
        {{"ciao", Tree{{}, {}, true}},
        {"bye", Tree{{{"b", Tree{{}, {}, true}}}, {{"b", 2}}, false}},
        {"hello", Tree{{{"ciao", Tree{{}, {}, true}}, {"a", Tree{{}, {}, true}}},
                        {{"ciao", 20}, {"a", 1}},
                        false}}},
        {{"ciao", 40}, {"bye", 1}, {"hello", 1}},
        false};
    prune(tree, {0.2, 0.5, 1.0});
    const Tree expected =
        Tree{{{"ciao", Tree{{}, {}, true}},
              {"<*>", Tree{{{"<*>", Tree{{}, {}, true}}}, {{"<*>", 23}}, false}}},
            {{"ciao", 40}, {"<*>", 2}},
            false};

    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, PruneWithRule3) {

    Tree tree = Tree{{{"ciao", Tree{{}, {}, true}},
                      {"bye", Tree{{}, {}, true}},
                      {"hello", Tree{{}, {}, true}},
                      {"hello1", Tree{{}, {}, true}},
                      {"hello2", Tree{{}, {}, true}},
                      {"hello3", Tree{{}, {}, true}},
                      {"hello4", Tree{{}, {}, true}}},
                    {{"ciao", 3},
                      {"bye", 2},
                      {"hello", 1},
                      {"hello1", 1},
                      {"hello2", 1},
                      {"hello3", 1},
                      {"hello4", 1}},
                    false};

    prune(tree, {0.2, 0.8, 0.4});
    const Tree expected = Tree{{{"ciao", Tree{{}, {}, true}},
                                {"bye", Tree{{}, {}, true}},
                                {"<*>", Tree{{}, {}, true}}},
                              {{"ciao", 3}, {"bye", 2}, {"<*>", 5}},
                              false};

    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, GetTemplates) {
    Tree tree = Tree{
        {{"ciao", Tree{{}, {}, true}},
        {"bye", Tree{{{"b", Tree{{}, {}, true}}}, {{"b", 2}}, false}},
        {"hello", Tree{{{"ciao", Tree{{}, {}, true}}, {"a", Tree{{}, {}, true}}},
                        {{"ciao", 20}, {"a", 1}},
                        true}}},
        {{"ciao", 40}, {"bye", 1}, {"hello", 1}},
        false};

    const std::set<std::string> expected = {"ciao", "bye b", "hello",
                                            "hello ciao", "hello a"};
    const auto templates = getTemplates(tree);
    const std::set<std::string> actual(templates.begin(), templates.end());

    EXPECT_EQ(actual, expected);
}
