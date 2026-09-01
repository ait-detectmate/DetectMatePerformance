#include <gtest/gtest.h>
#include <stdexcept>


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
    "081109 203615 148 INFO dfs.DataNode$PacketResponder: PacketResponder 0 for block blk_-6952295868487656571 terminating",
    "081109 203615 148 INFO dfs.DataNode$PacketResponder: PacketResponder 0 for block blk_-6952295868487656571 terminating",
};

std::vector<std::string> sentences3 = {
    "- 1117838570 2005.06.03 R02-M1-N0-C:J12-U11 2005-06-03-15.42.50.363779 R02-M1-N0-C:J12-U11 RAS KERNEL INFO instruction cache parity error corrected",
    "- 1117838570 2005.06.03 R02-M1-N0-C:J12-U11 2005-06-03-15.42.50.527847 R02-M1-N0-C:J12-U11 RAS KERNEL INFO instruction cache parity error corrected",
    "- 1117838570 2005.06.03 R02-M1-N0-C:J12-U11 2005-06-03-15.42.50.675872 R02-M1-N0-C:J12-U11 RAS KERNEL INFO instruction cache parity error corrected",
};

std::vector<std::string> sentences4 = {
    "Jan 21 00:00:09 dnsmasq[3468]: query[A] 3x6-.596-.IunWTzebVlyAhhHj*ZfWjOBun1zAf*Wgpq-.YarqcF7oovex5JXZQp35nThgDU1Q3p3lT/-.DM6Vx/vcq3AkrO4Xh2kjojk8RCiDE2wjSv-.gY6ONv8eNmDck8gGwJ8fU3PPctbthfeDZT-.customers_2017.xlsx.email-19.kennedy-mendoza.info from 10.143.0.103",
    "Jan 21 00:00:09 dnsmasq[3468]: forwarded 3x6-.596-.IunWTzebVlyAhhHj*ZfWjOBun1zAf*Wgpq-.YarqcF7oovex5JXZQp35nThgDU1Q3p3lT/-.DM6Vx/vcq3AkrO4Xh2kjojk8RCiDE2wjSv-.gY6ONv8eNmDck8gGwJ8fU3PPctbthfeDZT-.customers_2017.xlsx.email-19.kennedy-mendoza.info to 192.168.231.254",
    "Jan 21 00:00:09 dnsmasq[3468]: reply 3x6-.596-.IunWTzebVlyAhhHj*ZfWjOBun1zAf*Wgpq-.YarqcF7oovex5JXZQp35nThgDU1Q3p3lT/-.DM6Vx/vcq3AkrO4Xh2kjojk8RCiDE2wjSv-.gY6ONv8eNmDck8gGwJ8fU3PPctbthfeDZT-.customers_2017.xlsx.email-19.kennedy-mendoza.info is 195.128.194.1682",
};

std::vector<std::string> sentences5 = {
    "2022-01-21 00:09:11 jhall/192.168.230.165:46011 TLS: soft reset sec=3308/3308 bytes=45748/-1 pkts=649/0",
    "2022-01-21 00:09:11 jhall/192.168.230.165:46011 VERIFY OK: depth=1, C=AT, ST=Vienna, L=Vienna, O=Some Organisation GmbH, CN=OpenVPN CA, emailAddress=admin@organisation.cyberrange.at",
    "2022-01-21 00:09:11 jhall/192.168.230.165:46011 VERIFY KU OK",
};


TEST(AutoTest, GetTemplates) {
    // Note that we depend that the paths are sorted same way as logTypes var
    Templates result = getTemplates("Audit", paths);
    std::string temp = result.getNextConcatenate();
    EXPECT_EQ(temp, "saddr VAR");
}


TEST(AutoTest, GetTemplatesNotFound) {
    EXPECT_THROW(getTemplates("Unknown", paths);, std::runtime_error);
}


TEST(AutoTest, MainPipeline) {
    auto result1 = autoParserGenerator(sentences1, paths, regexs);
    Templates templates = result1.first;
    std::string temp = templates.getNextConcatenate();
    EXPECT_EQ(temp, "saddr VAR");


    auto result2 = autoParserGenerator(sentences2, paths, regexs);
    Templates templates2 = result2.first;
    std::string temp2 = templates2.getNextConcatenate();
    EXPECT_EQ(temp2, "Receiving block VAR src VAR dest VAR");
}


TEST(AutoTest, AllLogs) {
    std::string path = "../src/detectmateperformance/_templates/";

    auto result1 = doAutoParse(sentences1, path);
    Templates templates = result1.first;
    std::string temp = templates.getNextConcatenate();
    EXPECT_EQ(temp, "saddr VAR");

    auto result2 = doAutoParse(sentences2, path);
    Templates templates2 = result2.first;
    std::string temp2 = templates2.getNextConcatenate();
    EXPECT_EQ(temp2, "Receiving block VAR src VAR dest VAR");

    auto result3 = doAutoParse(sentences3, path);
    Templates templates3 = result3.first;
    std::string temp3 = templates3.getNextConcatenate();
    EXPECT_EQ(temp3, "VAR BGLERR_IDO_PKT_TIMEOUT connection lost to node/link/service card");

    auto result4 = doAutoParse(sentences4, path);
    Templates templates4 = result4.first;
    std::string temp4 = templates4.getNextConcatenate();
    EXPECT_EQ(temp4, "nameserver VAR refused to do a recursive query");

    auto result5 = doAutoParse(sentences5, path);
    Templates templates5 = result5.first;
    std::string temp5 = templates5.getNextConcatenate();
    EXPECT_EQ(temp5, "Validating certificate extended key usage");
}


TEST(AutoTest, AssignedLog) {
    std::string path = "../src/detectmateperformance/_templates/";

    auto result1 = doAutoParse(sentences1, path, "Audit");
    Templates templates = result1.first;
    std::string temp = templates.getNextConcatenate();
    EXPECT_EQ(temp, "saddr VAR");
    EXPECT_EQ(result1.second, 0);

    EXPECT_THROW(doAutoParse(sentences1, path, "Unknow"), std::runtime_error);

}
