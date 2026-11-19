#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

#include "../../src/detectmateperformance/_core/parsers/aecid_pg.h"

TEST(TreeOpTest, AddLog) {
    AecidPgTree tree = initTree();
    addLog(tree, {"hello", "there"});
    const AecidPgTree expected = AecidPgTree{
        {{"hello", AecidPgTree{{{"there", AecidPgTree{{}, {}, true}}},
                              {{"there", 1}}, false}}},
        {{"hello", 1}},
        false};

    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, AddLogs) {
    AecidPgTree tree = initTree();
    const AecidPgTree expected =
        AecidPgTree{{{"hello",
              AecidPgTree{{{"there", AecidPgTree{{}, {}, true}},
                           {"ciao", AecidPgTree{{}, {}, true}}},
                    {{"there", 1}, {"ciao", 1}},
                    false}}},
            {{"hello", 2}},
            false};

    const std::vector<std::string> logs = {"hello there", "hello ciao"};
    addLogs(tree, logs);
    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, MergeLeft) {
    AecidPgTree tree_1 =
        AecidPgTree{{{"hello",
              AecidPgTree{{{"there", AecidPgTree{{}, {}, true}},
                           {"ciao", AecidPgTree{{}, {}, true}}},
                    {{"there", 1}, {"ciao", 1}},
                    false}}},
            {{"hello", 2}},
            false};
    AecidPgTree tree_2 = AecidPgTree{
        {{"ciao", AecidPgTree{{}, {}, true}},
        {"hello", AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}},
                              {"a", AecidPgTree{{}, {}, true}}},
                             {{"ciao", 2}, {"a", 1}}, false}}},
        {{"ciao", 1}, {"hello", 1}},
        false};
    const AecidPgTree expected =
        AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}},
              {"hello", AecidPgTree{{{"there", AecidPgTree{{}, {}, true}},
                              {"ciao", AecidPgTree{{}, {}, true}},
                              {"a", AecidPgTree{{}, {}, true}}},
                            {{"there", 1}, {"ciao", 3}, {"a", 1}},
                            false}}},
            {{"ciao", 1}, {"hello", 3}},
            false};

    mergeLeft(tree_1, tree_2);
    EXPECT_EQ(tree_1, expected);
}

TEST(TreeOpTest, PruneWithRule1) {
    AecidPgTree tree = AecidPgTree{
        {{"hello",
          AecidPgTree{{{"a", AecidPgTree{{}, {}, true}},
                {"b", AecidPgTree{{}, {}, true}},
                {"c", AecidPgTree{{}, {}, true}},
                {"d", AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}}},
                                  {{"ciao", 1}}, false}},
                {"e", AecidPgTree{{}, {}, true}}},
              {{"a", 2}, {"b", 2}, {"c", 2}, {"d", 2}, {"e", 1}},
              false}}},
        {{"hello", 4}},
        false};
    prune(tree, {0.2, 0.8, 1.0});
    const AecidPgTree expected =
        AecidPgTree{{{"hello",
              AecidPgTree{{{"<*>",
                      AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}}},
                                   {{"ciao", 1}}, true}}},
                    {{"<*>", 9}},
                    false}}},
            {{"hello", 4}},
            false};

    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, PruneWithRule2) {

    AecidPgTree tree = AecidPgTree{
        {{"ciao", AecidPgTree{{}, {}, true}},
        {"bye", AecidPgTree{{{"b", AecidPgTree{{}, {}, true}}},
                           {{"b", 2}}, false}},
        {"hello", AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}},
                              {"a", AecidPgTree{{}, {}, true}}},
                             {{"ciao", 20}, {"a", 1}}, false}}},
        {{"ciao", 40}, {"bye", 1}, {"hello", 1}},
        false};
    prune(tree, {0.2, 0.5, 1.0});
    const AecidPgTree expected =
        AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}},
              {"<*>", AecidPgTree{{{"<*>", AecidPgTree{{}, {}, true}}},
                                  {{"<*>", 23}}, false}}},
            {{"ciao", 40}, {"<*>", 2}},
            false};

    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, PruneWithRule3) {

    AecidPgTree tree = AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}},
                      {"bye", AecidPgTree{{}, {}, true}},
                      {"hello", AecidPgTree{{}, {}, true}},
                      {"hello1", AecidPgTree{{}, {}, true}},
                      {"hello2", AecidPgTree{{}, {}, true}},
                      {"hello3", AecidPgTree{{}, {}, true}},
                      {"hello4", AecidPgTree{{}, {}, true}}},
                    {{"ciao", 3},
                      {"bye", 2},
                      {"hello", 1},
                      {"hello1", 1},
                      {"hello2", 1},
                      {"hello3", 1},
                      {"hello4", 1}},
                    false};

    prune(tree, {0.2, 0.8, 0.4});
    const AecidPgTree expected =
        AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}},
                    {"bye", AecidPgTree{{}, {}, true}},
                    {"<*>", AecidPgTree{{}, {}, true}}},
                    {{"ciao", 3}, {"bye", 2}, {"<*>", 5}},
                    false};

    EXPECT_EQ(tree, expected);
}

TEST(TreeOpTest, GetTemplates) {
    AecidPgTree tree = AecidPgTree{
        {{"ciao", AecidPgTree{{}, {}, true}},
        {"bye", AecidPgTree{{{"b", AecidPgTree{{}, {}, true}}},
                           {{"b", 2}}, false}},
        {"hello", AecidPgTree{{{"ciao", AecidPgTree{{}, {}, true}},
                              {"a", AecidPgTree{{}, {}, true}}},
                             {{"ciao", 20}, {"a", 1}}, true}}},
        {{"ciao", 40}, {"bye", 1}, {"hello", 1}},
        false};

    const std::set<std::string> expected = {"ciao", "bye b", "hello",
                                            "hello ciao", "hello a"};
    const auto templates = getTemplates(tree);
    const std::set<std::string> actual(templates.begin(), templates.end());

    EXPECT_EQ(actual, expected);
}
