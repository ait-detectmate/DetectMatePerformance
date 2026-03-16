#include <gtest/gtest.h>

#include "../../src/detectmateperformance/_core/template_matcher/tree.h"


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
    Tree* bchild2 = new Tree("there", "hi");

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
