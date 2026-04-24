#include <gtest/gtest.h>

#include "../../src/detectmateperformance/_core/aux.h"

#include "../../src/detectmateperformance/_core/_type/templates.h"
#include "../../src/detectmateperformance/_core/_type/parsed.h"
#include "../../src/detectmateperformance/_core/_type/element.h"


TEST(MessagesTest, Preprocessing) {
    std::string input = "Hello, world";
    auto result = preprocessing(input);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "Hello");
    EXPECT_EQ(result[1], "world");
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

    auto message1 = templates.getNext();
    EXPECT_EQ(message1.size(), 4);
    EXPECT_EQ(message1[0], "Hello");
    EXPECT_EQ(message1[1], "VAR");
    EXPECT_EQ(message1[2], "world");
    EXPECT_EQ(message1[3], "VAR");

    auto message2 = templates.getNext();
    EXPECT_EQ(message2.size(), 2);
    EXPECT_EQ(message2[0], "Goodbye");
    EXPECT_EQ(message2[1], "VAR");

    auto message3 = templates.getNext();
    EXPECT_TRUE(message3.empty());
}

TEST(ParsedMessagesTest, ParsedElementtest) {
    ParsedElement* parsed = new ParsedElement(
        0, "VAR template VAR VAR", "var1 var2 var3"
    );
    std::deque<std::string> expected = {"var1", "var2", "var3"};

    EXPECT_EQ(parsed->event_id, 0);
    EXPECT_EQ(parsed->log_template, "<*> template <*> <*>");
    EXPECT_EQ(parsed->variables, expected);
}

TEST(ParsedMessagesTest, Initialization) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates* templates = new Templates(input);
    ParsedMessages parsed(templates, 1);

    EXPECT_EQ(parsed.size(), 1);
    EXPECT_EQ(parsed.shape(), std::make_pair(1, 0));

    parsed.setElem(0, "Hello VAR world VAR");

    EXPECT_EQ(parsed.size(), 1);
    EXPECT_EQ(parsed.shape(), std::make_pair(1, 0));
}

TEST(ParsedMessagesTest, GetNext) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates* templates = new Templates(input);
    ParsedMessages parsed(templates, 4);


    parsed.setElem(0, "Hello VAR world VAR");
    parsed.setElem(1, "Hello VAR world VAR");
    parsed.setElem(2, "Goodbye VAR");
    parsed.setElem(3, "random thing");

    std::string temp1 = "Hello VAR world VAR";
    std::string temp2 = "Goodbye VAR";
    std::string temp3 = "template not found";

    EXPECT_EQ(parsed.getElem(0), temp1);
    EXPECT_EQ(parsed.getElem(1), temp1);
    EXPECT_EQ(parsed.getElem(2), temp2);
    EXPECT_EQ(parsed.getElem(3), temp3);
}

TEST(ParsedMessagesTest, GetAllIDs) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates* templates = new Templates(input);
    ParsedMessages parsed(templates, 4);

    EXPECT_EQ(parsed.getAllIDs().size(), 4);
}

TEST(ParsedMessagesTest, GetAllNext) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates* templates = new Templates(input);
    ParsedMessages parsed(templates, 4);

    parsed.setElem(0, "Hello VAR world VAR");
    parsed.setElem(1, "Hello VAR world VAR");
    parsed.setElem(2, "Goodbye VAR");
    parsed.setElem(3, "random thing");

    std::vector<std::string> results = parsed.getAllElemts();
    for (int i = 0; i < results.size(); i++) {
        EXPECT_EQ(parsed.getElem(i), results[i]);
    }

}

TEST(ParsedMessagesTest, GetNextWithVar) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates* templates = new Templates(input);
    ParsedMessages parsed(templates, 4);

    std::string var1 = "";
    std::string var2 = "a b";

    parsed.setElemWithVar(0, "Hello VAR world VAR", var1);
    parsed.setElemWithVar(1, "Hello VAR world VAR", var2);
    parsed.setElemWithVar(2, "Goodbye VAR", var2);
    parsed.setElemWithVar(3, "random thing", var1);

    std::string temp1 = "Hello VAR world VAR";
    std::string temp2 = "Goodbye VAR";
    std::string temp3 = "template not found";
    std::string evar1 = "";
    std::string evar2 = "a b";

    EXPECT_EQ(parsed.getElemWithVar(0).first, temp1);
    EXPECT_EQ(parsed.getElemWithVar(0).second, evar1);
    EXPECT_EQ(parsed.getElemWithVar(1).first, temp1);
    EXPECT_EQ(parsed.getElemWithVar(1).second, evar2);
    EXPECT_EQ(parsed.getElemWithVar(2).first, temp2);
    EXPECT_EQ(parsed.getElemWithVar(2).second, evar2);
    EXPECT_EQ(parsed.getElemWithVar(3).first, temp3);
    EXPECT_EQ(parsed.getElemWithVar(3).second, evar1);
}

TEST(ParsedMessagesTest, GetAllVar) {
    std::deque<std::string> input = {"Hello VAR, world=VAR:VAR VAR", "Goodbye VAR"};
    Templates* templates = new Templates(input);
    ParsedMessages parsed(templates, 4);

    std::string var1 = "";
    std::string var2 = "a b";

    parsed.setElemWithVar(0, "Hello VAR world VAR", var1);
    parsed.setElemWithVar(1, "Hello VAR world VAR", var2);
    parsed.setElemWithVar(2, "Goodbye VAR", var2);
    parsed.setElemWithVar(3, "random thing", var1);

    std::vector<std::string> result = parsed.getAllVar();

    for (int i = 0; i < result.size(); i++) {
        EXPECT_EQ(parsed.getElemWithVar(i).second, result[i]);
    }
}
