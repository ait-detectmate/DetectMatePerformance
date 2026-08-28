#include <gtest/gtest.h>


#include "../../src/detectmateperformance/_core/parsers/auto_parser.h"


std::vector<std::string> paths = {
    "../src/detectmateperformance/_templates/audit_templates.txt",
    "../src/detectmateperformance/_templates/hdfs_templates.txt",
};

std::vector<const char*> regexs = {
    R"(type=(\w+) msg=audit\(([^:]+):(\d+)\): (.*))",
    R"((\S+) (\S+) (\d+) (\S+) ([^:]+): (.*))",
};

std::vector<std::string> sentences1 = {
    "type=CRED_ACQ msg=audit(1642723741.072:376): pid=10125 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=65 res=1",
    "type=CRED_ACQ msg=audit(1642723741.072:376): pid=10125 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=65 res=1",
    "type=CRED_ACQ msg=audit(1642723741.072:376): pid=10125 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=65 res=1",
};

std::vector<std::string> sentences2 = {
    "081109 203615 148 INFO dfs.DataNode$PacketResponder: PacketResponder 1 for block blk_38865049064139660 terminating",
    "081109 203615 148 INFO dfs.DataNode$PacketResponder: PacketResponder 0 for block blk_-6952295868487656571 terminating"
    "081109 203615 148 INFO dfs.DataNode$PacketResponder: PacketResponder 0 for block blk_-6952295868487656571 terminating",
};

TEST(AutoTest, MainPipeline) {
    Templates templates = AutoParserGenerator(sentences1, paths, regexs);
    std::string temp = templates.getNextConcatenate();
    EXPECT_EQ(temp, "saddr VAR");

    Templates templates2 = AutoParserGenerator(sentences2, paths, regexs);
    std::string temp2 = templates2.getNextConcatenate();
    EXPECT_EQ(temp2, "Receiving block VAR src VAR dest VAR");
}
