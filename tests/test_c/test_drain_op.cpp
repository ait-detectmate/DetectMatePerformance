#include <gtest/gtest.h>

#include <string>
#include <cmath>
#include "../../src/detectmateperformance/_core/parsers/drain.h"



TEST(DrainTest, Similarity) {
    std::string string_1 = "Hello, tomas world";
    std::string string_2 = "Hello, alice world";
    std::string string_3 = "Hello";
    std::string string_4 = "Ciao";

    float result = calculate_sim(string_1, string_2);
    EXPECT_TRUE(0.71 < result && result < 0.73);

    float result_2 = calculate_sim(string_1, string_1);
    EXPECT_EQ(result_2, 1.0);

    float result_3 = calculate_sim(string_3, string_4);
    EXPECT_EQ(result_3, 0.0);

    float result_4 = calculate_sim(string_3, string_1);
    EXPECT_EQ(result_4, 1.0);
}


TEST(DrainTest, GenerateTemplate) {

    std::deque<std::string> inp = {
        "Hello tobias my man",
        "Hello guillermo my man",
        "Hello julie my woman",
    };
    std::string expected = "Hello <*> my <*>";

    EXPECT_EQ(generate_template(inp), expected);

    std::deque<std::string> inp2 = {
        "Hello tobias my man my bro",
        "Hello guillermo my man",
        "Hello julie my woman a",
    };
    std::string expected2 = "Hello <*> my <*>";

    EXPECT_EQ(generate_template(inp2), expected2);

}


TEST(DrainTest, GenerateMultipleTemplate) {

    std::vector<std::deque<std::string>> inp = {
        {
            "Hello tobias my man",
            "Hello guillermo my man",
            "Hello julie my woman",
            "Mamma mia here again",
            "Mamma tia here again"
        },
        {
            "ciao bella"
        }
    };
    std::deque<std::string> expected = {"Hello <*> my <*>"};

    std::deque<std::string> result = generate_templates(inp, 0.2);

    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "Hello <*> my <*>");
    EXPECT_EQ(result[1], "Mamma <*> here again");
    EXPECT_EQ(result[2], "ciao bella");

}


TEST(DrainTest, CleanTemplates) {
    std::deque<std::string> templates = {
        "Hello there <*> <*> kenobi",
        "Hello there <*> <*>",
        "Hello there <*> <*> kenobi",
        "<*>"
    };
    std::deque<std::string> expected = {
        "Hello there <*> kenobi",
        "Hello there <*> ",
    };

    EXPECT_EQ(clean_templates(templates), expected);
}
