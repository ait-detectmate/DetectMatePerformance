#include <gtest/gtest.h>

#include <string>
#include <cmath>
#include "../../src/detectmateperformance/_core/parsers/drain.h"



TEST(DrainTest, Similarity) {
    std::string string_1 = "Hello, tomas world";
    std::string string_2 = "Hello, alice world";
    std::string string_3 = "Hello";
    std::string string_4 = "Ciao";

    float result = calculateSim(string_1, string_2);
    EXPECT_TRUE(0.71 < result && result < 0.73);

    float result_2 = calculateSim(string_1, string_1);
    EXPECT_EQ(result_2, 1.0);

    float result_3 = calculateSim(string_3, string_4);
    EXPECT_EQ(result_3, 0.0);

    float result_4 = calculateSim(string_3, string_1);
    EXPECT_EQ(result_4, 1.0);
}


TEST(DrainTest, GenerateTemplate) {

    std::deque<std::string> inp = {
        "Hello tobias my man",
        "Hello guillermo my man",
        "Hello julie my woman",
    };
    std::string expected = "Hello VAR my VAR";

    EXPECT_EQ(generateTemplate(inp), expected);

    std::deque<std::string> inp2 = {
        "Hello tobias my man my bro",
        "Hello guillermo my man",
        "Hello julie my woman a",
    };
    std::string expected2 = "Hello VAR my VAR";

    EXPECT_EQ(generateTemplate(inp2), expected2);

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
    std::deque<std::string> expected = {"Hello VAR my VAR"};

    std::deque<std::string> result = generateTemplates(inp, 0.2);

    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "Hello VAR my VAR");
    EXPECT_EQ(result[1], "Mamma VAR here again");
    EXPECT_EQ(result[2], "ciao bella");

}


TEST(DrainTest, CleanTemplates) {
    std::deque<std::string> templates = {
        "Hello there VAR VAR kenobi",
        "Hello there VAR VAR",
        "Hello there VAR VAR kenobi",
        "VAR"
    };
    std::deque<std::string> expected = {
        "Hello there VAR kenobi",
        "Hello there VAR ",
    };

    EXPECT_EQ(cleanTemplates(templates), expected);
}


TEST(DrainTest, DrainGenerator) {

    std::vector<std::deque<std::string>> inp = {
        {
            "hello tobias my man number 2",
            "hello guillermo my man number 2",
            "hello julie my woman number 2,"
        },
    };

    Templates result = drainGenerator(inp, 0.2);
    auto message1 = result.getNext();

    for (size_t i= 0; i < message1.size(); i++)
        std::cout << message1[i] << std::endl;

    EXPECT_EQ(message1.size(), 6);
    EXPECT_EQ(message1[0], "hello");
    EXPECT_EQ(message1[1], "VAR");
    EXPECT_EQ(message1[2], "my");
    EXPECT_EQ(message1[3], "VAR");
    EXPECT_EQ(message1[4], "number");
    EXPECT_EQ(message1[5], "VAR");

}
