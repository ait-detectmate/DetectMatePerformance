#include <gtest/gtest.h>

#include "../../src/_core/_type/message.h"
#include "../../src/_core/_type/templates.h"
#include "../../src/_core/_type/parsed.h"

#include "../../src/_core/template_matcher/tree.h"
#include "../../src/_core/template_matcher/variables.h"


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

TEST(TreeTest, Initialization) {
    Tree* node = new Tree("hi", "hello");

    EXPECT_EQ(node->getData(), "hi");
    EXPECT_TRUE(node->isFullTemplate());
    delete node; 
}

TEST(TreeTest, AddChild) {
    Tree* parent = new Tree("");
    Tree* child = new Tree("hi", "hi");

    parent->addChild(child);
    EXPECT_EQ(parent->getChildren().size(), 1);
    delete parent;  
}

TEST(TreeTest, SetTemplate) {
    Tree* parent = new Tree("hi");
    EXPECT_FALSE(parent->isFullTemplate());

    parent->setTemplate("ciaoo");
    EXPECT_TRUE(parent->isFullTemplate());
    EXPECT_EQ(parent->getTemplate(), "ciaoo");

    delete parent;  
}

TEST(TreeTest, GetChildrenValue) {
    Tree* parent = new Tree("");
    Tree* child1 = new Tree("hi");
    Tree* child2 = new Tree("ciao", "ciao");

    parent->addChild(child1);
    parent->addChild(child2);

    EXPECT_EQ(parent->getChildren()[0]->getData(), "hi");
    EXPECT_EQ(parent->getChildren()[1]->getData(), "ciao");
    EXPECT_FALSE(parent->isFullTemplate());

    delete parent;
}

TEST(TreeTest, Contains) {
    Tree* parent = new Tree("");
    Tree* child1 = new Tree("hi");
    Tree* child2 = new Tree("ciao", "ciao");

    parent->addChild(child1);
    parent->addChild(child2);
    
    std::pair<bool, Tree*> result1 = parent->contains("hi");
    EXPECT_TRUE(result1.first);

    std::pair<bool, Tree*> result2 = parent->contains("kenobi");
    EXPECT_FALSE(result2.first);

    delete parent;
}

TEST(TreeTest, ContainsVariable) {
     Tree* parent = new Tree("");
    Tree* child1 = new Tree("hi");
    Tree* grandchild1 = new Tree("VAR", "hi VAR");

    parent->addChild(child1);
    child1->addChild(grandchild1);
    
    std::pair<bool, Tree*> result1 = parent->contains_variable();
    EXPECT_FALSE(result1.first);

    std::pair<bool, Tree*> result2 = child1->contains_variable();
    EXPECT_TRUE(result2.first);

    delete parent;
}

TEST(TreeTest, IsEqual) {
    Tree* aparent = new Tree("");
    Tree* achild1 = new Tree("hi");
    Tree* achild2 = new Tree("there", "hi there");

    aparent->addChild(achild1);
    aparent->addChild(achild2);

    Tree* bparent = new Tree("");
    Tree* bchild1 = new Tree("hi");
    Tree* bchild2 = new Tree("there");

    bparent->addChild(bchild1);
    bparent->addChild(bchild2);

    Tree* cparent = new Tree("");
    Tree* cchild1 = new Tree("hi");
    Tree* cchild2 = new Tree("there", "hi there");

    cparent->addChild(cchild1);
    cparent->addChild(cchild2);

    Tree* dparent = new Tree("");
    Tree* dchild1 = new Tree("hi");

    dparent->addChild(dchild1);

    EXPECT_FALSE(aparent->isEqual(bparent));
    EXPECT_TRUE(aparent->isEqual(cparent));
    EXPECT_FALSE(aparent->isEqual(dparent));

    delete aparent;
    delete bparent;
    delete cparent;
    delete dparent;
}


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