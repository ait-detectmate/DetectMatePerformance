#include <gtest/gtest.h>

#include "../../src/_core/message.h"


TEST(MessagesTest, Preprocessing) {
    std::string input = "Hello, world!";
    auto result = preprocessing(input);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "Hello,");
    EXPECT_EQ(result[1], "world!");
}

TEST(MessagesTest, GetNextMessage) {
    std::deque<std::string> input = {"Hello, world!", "Goodbye, world!"};
    Messages messages(input);

    auto message1 = messages.getNextMessage();
    EXPECT_EQ(message1.size(), 2);
    EXPECT_EQ(message1[0], "Hello,");
    EXPECT_EQ(message1[1], "world!");

    auto message2 = messages.getNextMessage();
    EXPECT_EQ(message2.size(), 2);
    EXPECT_EQ(message2[0], "Goodbye,");
    EXPECT_EQ(message2[1], "world!");

    auto message3 = messages.getNextMessage();
    EXPECT_TRUE(message3.empty());
}

