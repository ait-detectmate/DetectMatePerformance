#include <gtest/gtest.h>

#include "../../src/detectmateperformance/_core/aux.h"

#include "../../src/detectmateperformance/_core/_type/templates.h"
#include "../../src/detectmateperformance/_core/_type/parsed.h"
#include "../../src/detectmateperformance/_core/_type/element.h"

#include "../../src/detectmateperformance/_core/template_matcher/tree.h"
#include "../../src/detectmateperformance/_core/template_matcher/variables.h"
#include "../../src/detectmateperformance/_core/template_matcher/tree_op.h"
#include "../../src/detectmateperformance/_core/template_matcher/match_tree.h"


TEST(TreeOpTest, SearchTree) {
    Tree* root = new Tree("");
    Tree* child1 = new Tree("hello");
    Tree* child2 = new Tree("hi", "hi");
    Tree* grandchild = new Tree("there");
    Tree* grandchild2 = new Tree("kenobi", "hello there kenobi");

    root->addChild(child1);
    root->addChild(child2);
    child1->addChild(grandchild);
    grandchild->addChild(grandchild2);

    Variables* vars = new Variables();

    std::deque<std::string> sequence0 = {"hello", "there"};
    std::pair<bool, Tree*> result0 = searchTree(root, sequence0, vars);
    EXPECT_FALSE(result0.first);

    delete root;
    delete vars;
}


TEST(TreeOpTest, SearchTreeWithVariable) {
    Tree* root = new Tree("");
    Tree* child1 = new Tree("hello");
    Tree* child2 = new Tree("hi");
    Tree* grandchild0 = new Tree("VAR", "hi VAR");
    Tree* grandchild = new Tree("VAR");
    Tree* grandchild2 = new Tree("kenobi", "hello VAR kenobi");
    Tree* grandchild3 = new Tree("VAR");
    Tree* grandchild4 = new Tree("you", "hello VAR kenobi VAR you");

    root->addChild(child1);
    root->addChild(child2);
    child2->addChild(grandchild0);
    child1->addChild(grandchild);
    grandchild->addChild(grandchild2);
    grandchild2->addChild(grandchild3);
    grandchild3->addChild(grandchild4);

    Variables* vars1 = new Variables();
    std::deque<std::string> sequence1 = {"hello", "there", "mr.", "kenobi"};
    auto result1 = searchTree(root, sequence1, vars1);
    EXPECT_TRUE(result1.first);

    Variables* vars2 = new Variables();
    std::deque<std::string> sequence2 = {"hello", "there", "mr.", "jonathan"};
    auto result2 = searchTree(root, sequence2, vars2);
    EXPECT_FALSE(result2.first);

    Variables* vars3 = new Variables();
    std::deque<std::string> sequence3 = {"hello", "there", "mr.", "kenobi", "nice", "to", "meet", "you"};
    auto result3 = searchTree(root, sequence3, vars3);
    EXPECT_TRUE(result3.first);

    Variables* vars4 = new Variables();
    std::deque<std::string> sequence4 = {"hi", "there", "mr.", "asdads"};
    auto result4 = searchTree(root, sequence4, vars4);
    EXPECT_TRUE(result4.first);

    Variables* vars5 = new Variables();
    std::deque<std::string> sequence5 = {"hello", "there", "mr.", "kenobi", "nice", "to", "meet"};
    auto result5 = searchTree(root, sequence5, vars5);
    EXPECT_FALSE(result5.first);

    std::string expected1 = "there mr.";
    EXPECT_EQ(vars1->export_variables(), expected1);

    std::string expected2 = "there mr. nice to meet";
    EXPECT_EQ(vars3->export_variables(), expected2);

    std::string expected3 = "there mr. asdads";
    EXPECT_EQ(vars4->export_variables(), expected3);

    delete root;
    delete vars1;
    delete vars2;
    delete vars3;
    delete vars4;
    delete vars5;
}


TEST(TreeOpTest, AddSequence) {
    Tree* root = new Tree("");
    Tree* child1 = new Tree("hi");
    Tree* grandchild = new Tree("VAR", "hi VAR");

    root->addChild(child1);
    child1->addChild(grandchild);

    std::vector<std::string> sequence = {"hi", "VAR"};
    Tree* root2 = new Tree("");
    addSequence(root2, sequence, "hi VAR", 0);
    std::cout << root2->getData() << std::endl;

    EXPECT_TRUE(root->isEqual(root2));

    delete root;
    delete root2;
}

TEST(TreeOpTest, AddSequenceSpecialCase) {
    // TODO: decide if this is need it in the future
    Tree* root = new Tree("");
    Tree* child1 = new Tree("hi");
    Tree* grandchild = new Tree("VAR", "hi VAR");

    root->addChild(child1);
    child1->addChild(grandchild);

    std::vector<std::string> sequence = {"hi", "VAR", "VAR", "VAR"};
    Tree* root2 = new Tree("");
    addSequence(root2, sequence, "hi VAR", 0);

    EXPECT_TRUE(root->isEqual(root2));

    delete root;
    delete root2;
}

TEST(TreeOpTest, BuiltTree) {
    Tree* root = new Tree("");
    Tree* child1 = new Tree("hi");
    Tree* grandchild0 = new Tree("there", "hi there");
    Tree* grandchild1 = new Tree("general");
    Tree* grandchild2 = new Tree("kenobi", "hi general kenobi");

    root->addChild(child1);
    child1->addChild(grandchild0);
    child1->addChild(grandchild1);
    grandchild1->addChild(grandchild2);

    std::deque<std::string> sequences = {"hi there", "hi general kenobi"};
    Templates* temp = new Templates(sequences);
    Tree* root2 = buildTree(temp);

    EXPECT_TRUE(root->isEqual(root2));

    delete root;
    delete root2;
}

TEST(TreeMatchTest, Initializetree) {
    Tree* root = new Tree("");
    Tree* child1 = new Tree("hi");
    Tree* grandchild0 = new Tree("there", "hi there");
    Tree* grandchild1 = new Tree("general");
    Tree* grandchild2 = new Tree("kenobi", "hi general kenobi");

    root->addChild(child1);
    child1->addChild(grandchild0);
    child1->addChild(grandchild1);
    grandchild1->addChild(grandchild2);

    std::deque<std::string> sequences = {"hi there", "hi general kenobi"};
    Templates* temp = new Templates(sequences);
    MatchTree* matcher = new MatchTree(temp);

    EXPECT_TRUE(matcher->isEqual(root));

    delete matcher;
    delete root;
}

TEST(TreeMatchTree, getTemplates) {
    std::deque<std::string> sequences = {"hi there", "hi general kenobi"};
    Templates* temp = new Templates(sequences);
    MatchTree* matcher = new MatchTree(temp);

    std::deque<std::string> sequences2 = {"hi there", "hi general kenobi"};
    Templates* temp2 = new Templates(sequences2);

    auto match_temp = matcher->getTemplates();
    match_temp->resetCount();
    EXPECT_EQ(temp2->shape(), match_temp->shape());

    for (size_t i = 0; i < temp2->size(); i++) {
        auto value1 = match_temp->getNext();
        auto value2 = temp2->getNext();

        EXPECT_EQ(value1, value2);
    }

}

TEST(TreeMatchTest, EqualMatcher) {
    std::deque<std::string> sequences = {"hi there", "hi general kenobi"};
    Templates* temp1 = new Templates(sequences);
    MatchTree* matcher = new MatchTree(temp1);

    std::deque<std::string> sequences2 = {"hi there", "hi general kenobi"};
    Templates* temp2 = new Templates(sequences2);
    MatchTree* matcher2 = new MatchTree(temp2);

    std::deque<std::string> sequences3 = {"hi", "hi general kenobi"};
    Templates* temp3 = new Templates(sequences3);
    MatchTree* matcher3 = new MatchTree(temp3);

    EXPECT_TRUE(matcher->isEqual(matcher2));
    EXPECT_FALSE(matcher->isEqual(matcher3));

    delete matcher;
    delete matcher2;
    delete matcher3;
}

TEST(TreeMatchTest, MatchString) {
    std::deque<std::string> sequences = {
        "hi there", "hi general VAR VAR kenobi", "load VAR from VAR VAR"
    };
    Templates* temp = new Templates(sequences);
    MatchTree* matcher = new MatchTree(temp);

    ParsedMessages* result1 = matcher->match_string("hi there");
    EXPECT_EQ(result1->size(), 1);
    EXPECT_EQ(result1->getElem(0).log_template, "hi there");

    ParsedMessages* result2 = matcher->match_string("hi general mr. and mrs. kenobi");
    EXPECT_EQ(result2->size(), 1);
    EXPECT_EQ(result2->getElem(0).log_template, "hi general <*> kenobi");

    ParsedMessages* result3 = matcher->match_string("hi random guy");
    EXPECT_EQ(result3->size(), 1);
    EXPECT_EQ(result3->getElem(0).log_template, "template not found");

    delete matcher;
}

TEST(TreeMatchTest, MatchStringWithVar) {
    std::deque<std::string> sequences = {
        "hi there", "hi general VAR VAR kenobi", "load VAR from VAR VAR"
    };
    Templates* temp = new Templates(sequences);
    MatchTree* matcher = new MatchTree(temp);

    ParsedElement result1 = matcher->match_string_with_var("hi there")->getElemWithVar(0);
    std::deque<std::string> expected1 = {};
    EXPECT_EQ(result1.log_template, "hi there");
    EXPECT_EQ(result1.variables, expected1);

    ParsedElement result2 = matcher->match_string_with_var(
        "hi general mr. and mrs. kenobi"
    )->getElemWithVar(0);
    std::deque<std::string> expected2 = {"mr", "and", "mrs"};
    EXPECT_EQ(result2.log_template, "hi general <*> kenobi");
    EXPECT_EQ(result2.variables, expected2);

    ParsedElement result3 = matcher->match_string_with_var(
        "hi random guy"
    )->getElemWithVar(0);
    EXPECT_EQ(result3.log_template, "template not found");

    ParsedElement result4 = matcher->match_string_with_var(
        "load 1213 asd from 112 bye"
    )->getElemWithVar(0);
    std::deque<std::string> expected4 = {"1213", "asd", "112", "bye"};
    EXPECT_EQ(result4.log_template, "load <*> from <*>>");
    EXPECT_EQ(result4.variables, expected4);

    delete matcher;
}


TEST(TreeMatchTest, MatchStringBatch) {
    std::deque<std::string> sequences = {
        "hi there", "hi general VAR VAR kenobi", "load VAR from VAR VAR"
    };
    std::vector<std::string> logs = {
        "hi there", "hi general mr. and mrs. kenobi", "hi random guy", "load 1213 asd from 112 bye"
    };
    std::vector<std::string> msg = {
        "hi there", "hi general VAR kenobi", "template not found", "load VAR from VAR"
    };

    Templates* temp2 = new Templates(sequences);
    ParsedMessages* expected = new ParsedMessages(temp2, 4);
    for (int i = 0; i < msg.size(); i++) {
        expected->setElem(i, msg[i]);
    }

    Templates* temp = new Templates(sequences);
    MatchTree* matcher = new MatchTree(temp);
    ParsedMessages* results = matcher->match_batch(logs, 1);
    ParsedMessages* results_threats = matcher->match_batch(logs, 4);

    EXPECT_EQ(4, msg.size());
    for (int i = 0; i < msg.size(); i++) {
        EXPECT_EQ(expected->getElem(i).log_template, results->getElem(i).log_template);
        EXPECT_EQ(results_threats->getElem(i).log_template, results->getElem(i).log_template);
    }

    delete matcher;
}

TEST(TreeMatchTest, MatchStringBatchVar) {
    std::deque<std::string> sequences = {
        "hi there", "hi general VAR VAR kenobi", "load VAR from VAR VAR"
    };
    std::vector<std::string> logs = {
        "hi there", "hi general mr. and mrs. kenobi", "hi random guy", "load 1213 asd from 112 bye"
    };
    std::vector<std::string> msg = {
        "hi there", "hi general VAR kenobi", "template not found", "load VAR from VAR"
    };
    std::vector<std::string> msg_ex = {
        "hi there", "hi general <*> kenobi", "template not found", "load <*> from <*>"
    };
    std::vector<std::string> vector_vars = {
        "", "mr and mrs", "",  "1213 asd 112 bye"
    };
    std::vector<std::deque<std::string>> ex_vector_vars = {
        {}, {"mr", "and", "mrs"}, {},  {"1213", "asd", "112", "bye"}
    };

    Templates* temp2 = new Templates(sequences);
    ParsedMessages* expected = new ParsedMessages(temp2, 4);
    for (int i = 0; i < msg.size(); i++) {
        expected->setElemWithVar(i, msg[i], vector_vars[i]);
    }

    Templates* temp = new Templates(sequences);
    MatchTree* matcher = new MatchTree(temp);
    ParsedMessages* results = matcher->match_batch_with_var(logs, 1);
    ParsedMessages* results_threats = matcher->match_batch_with_var(logs, 4);

    EXPECT_EQ(4, msg.size());
    for (int i = 0; i < msg.size(); i++) {
        EXPECT_EQ(results_threats->getElemWithVar(i).variables, results->getElemWithVar(i).variables);
        ParsedElement aux = results->getElemWithVar(i);
        EXPECT_EQ(msg_ex[i], aux.log_template);
        EXPECT_EQ(aux.variables, ex_vector_vars[i]);
    }

    delete matcher;
}

TEST(ParsedMessagesTest, HardCases1) {
    std::deque<std::string> input = {
        "VAR floating point alignment exceptions",
        "floating point instr. enabled.....VAR",
        "floating pt ex mode <*> enable......VAR",
    };
    std::string log = "8 floating point alignment exceptions";

    Templates* templates = new Templates(input);
    MatchTree* matcher = new MatchTree(templates);

    ParsedMessages* result1 = matcher->match_string(log);
    EXPECT_EQ(result1->size(), 1);
    EXPECT_EQ(result1->getElem(0).log_template, "<*> floating point alignment exceptions");

}

TEST(ParsedMessagesTest, HardCases2) {
    std::deque<std::string> input = {
        "Exception in receiveBlock for block VAR java.io.IOException: Broken pipe",
        "VAR:Exception writing block VAR to mirror VAR",
    };
    std::string log = "10.250.14.196:50010:Exception writing block blk_-6994808880344424033 to mirror 10.251.107.50:50010";

    Templates* templates = new Templates(input);
    MatchTree* matcher = new MatchTree(templates);

    ParsedMessages* result1 = matcher->match_string(log);
    EXPECT_EQ(result1->size(), 1);
    EXPECT_EQ(result1->getElem(0).log_template, "<*> Exception writing block <*> to mirror <*>");

}

TEST(ParsedMessagesTest, HardCases3) {
    std::deque<std::string> input = {
        "rts: kernel terminated for reason VAR",
        "rts: kernel terminated for reason VAR: bad message header: invalid cpu, type=VAR, cpu=VAR, index=VAR, total=VAR",
    };
    std::string log = "rts: kernel terminated for reason 1001";

    Templates* templates = new Templates(input);
    MatchTree* matcher = new MatchTree(templates);

    ParsedMessages* result1 = matcher->match_string(log);
    EXPECT_EQ(result1->size(), 1);
    EXPECT_EQ(result1->getElem(0).log_template, "rts kernel terminated for reason <*>");

}

TEST(ParsedMessagesTest, HardCases4) {
    std::deque<std::string> input = {
        "data TLB error interrupt data address space................0",
        "data TLB error interrupt",
    };
    std::string log = "data TLB error interrupt";

    Templates* templates = new Templates(input);
    MatchTree* matcher = new MatchTree(templates);

    ParsedMessages* result1 = matcher->match_string(log);
    EXPECT_EQ(result1->size(), 1);
    EXPECT_EQ(result1->getElem(0).log_template, "data TLB error interrupt");

}
