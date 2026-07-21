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

    std::vector<std::string> inp = {
        "Hello tobias my man",
        "Hello guillermo my man",
        "Hello julie my woman",
    };
    std::string expected = "Hello <*> my <*>";

    EXPECT_EQ(generate_template(inp), expected);

    std::vector<std::string> inp2 = {
        "Hello tobias my man my bro",
        "Hello guillermo my man",
        "Hello julie my woman a",
    };
    std::string expected2 = "Hello <*> my <*>";

    EXPECT_EQ(generate_template(inp2), expected2);

}
