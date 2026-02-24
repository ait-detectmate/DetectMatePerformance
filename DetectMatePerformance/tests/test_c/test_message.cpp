#include <gtest/gtest.h>

#include "../../src/_core/_type/message.h"
#include "../../src/_core/_type/templates.h"
#include "../../src/_core/_type/parsed.h"


TEST(MessagesTest, Preprocessing) {
    std::string input = "Hello, world";
    auto result = preprocessing(input);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "Hello");
    EXPECT_EQ(result[1], "world");
}

TEST(MessagesTest, SizeShape) {
    std::deque<std::string> input = {"Hello, world!", "Goodbye, world! goodbye"};
    Messages messages(input);

    EXPECT_EQ(messages.size(), 2);
    EXPECT_EQ(messages.shape(), std::make_pair(2, 3));
}

TEST(MessagesTest, GetNextMessage) {
    std::deque<std::string> input = {"Hello, world=", "Goodbye:, VAR"};
    Messages messages(input);

    auto message1 = messages.getNextMessage();
    EXPECT_EQ(message1.size(), 2);
    EXPECT_EQ(message1[0], "Hello");
    EXPECT_EQ(message1[1], "world");

    auto message2 = messages.getNextMessage();
    EXPECT_EQ(message2.size(), 2);
    EXPECT_EQ(message2[0], "Goodbye");
    EXPECT_EQ(message2[1], "VAR");

    auto message3 = messages.getNextMessage();
    EXPECT_TRUE(message3.empty());
}

TEST(TemplatesTest, PreprocessingTemplates) {
    std::string input = "Hello VAR, world=VAR:VAR VAR";
    auto msg = preprocessing(input);
    auto result = tpreprocess(msg);

    EXPECT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "Hello");
    EXPECT_EQ(result[1], "VAR");
    EXPECT_EQ(result[2], "world");
    EXPECT_EQ(result[3], "VAR");
}

TEST(TemplatesTest, SizeShape) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates templates(input);

    EXPECT_EQ(templates.size(), 2);
    EXPECT_EQ(templates.shape(), std::make_pair(2, 4));
}

TEST(TemplatesTest, GetNextTemplate) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates templates(input);

    auto message1 = templates.getNextTemplate();
    EXPECT_EQ(message1.size(), 4);
    EXPECT_EQ(message1[0], "Hello");
    EXPECT_EQ(message1[1], "VAR");
    EXPECT_EQ(message1[2], "world");
    EXPECT_EQ(message1[3], "VAR");

    auto message2 = templates.getNextTemplate();
    EXPECT_EQ(message2.size(), 2);
    EXPECT_EQ(message2[0], "Goodbye");
    EXPECT_EQ(message2[1], "VAR");

    auto message3 = templates.getNextTemplate();
    EXPECT_TRUE(message3.empty());
}

TEST(ParsedMessagesTest, Initialization) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates templates(input);
    ParsedMessages parsed(templates);

    EXPECT_EQ(parsed.size(), 0);
    EXPECT_EQ(parsed.shape(), std::make_pair(0, 0));

    parsed.setNext({"Hello", "VAR", "world", "VAR"});

    EXPECT_EQ(parsed.size(), 1);
    EXPECT_EQ(parsed.shape(), std::make_pair(1, 0));
}

TEST(ParsedMessagesTest, GetNext) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates templates(input);
    ParsedMessages parsed(templates);

    parsed.setNext({"Hello", "VAR", "world", "VAR"});
    parsed.setNext({"Hello", "VAR", "world", "VAR"});
    parsed.setNext({"Goodbye", "VAR"});

    std::deque<std::string> temp1 = {"Hello", "VAR", "world", "VAR"};
    std::deque<std::string> temp2 = {"Goodbye", "VAR"};

    int i;
    i = 0;
    for (std::string s : parsed.getNext()) {
        EXPECT_EQ(s, temp1[i]);
        i++;
    }

    i = 0;
    for (std::string s : parsed.getNext()) {
        EXPECT_EQ(s, temp1[i]);
        i++;
    }

    i = 0;
    for (std::string s : parsed.getNext()) {
        EXPECT_EQ(s, temp2[i]);
        i++;
    }
}