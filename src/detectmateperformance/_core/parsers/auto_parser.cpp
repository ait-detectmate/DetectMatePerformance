
#include "auto_parser.h"

#include <stdexcept>
#include <regex>

#include <iostream>

std::vector<std::string> pathTemplates = {
    "audit_templates.txt",
    "bgl_templates.txt",
    "dnsmasq_templates.txt",
    "hdfs_templates.txt",
    "openvpn.txt",
    "syslog_templates.txt",
    "thunderbird_templates.txt",
};

std::vector<std::string> logTypes = {
    "Audit", "BGL", "DNSmasq", "HDFS" ,"OpenVPN", "SysLog", "Thunderbird"
};

std::vector<const char*> regexs_patterns = {
    R"(type=(\w+) msg=audit\(([^:]+):(\d+)\): (.*))",
    R"((\S+) (\S+) (\S+) (\S+) (\S+) (\S+) (\S+) (\S+) (\S+) (.*))",
    R"((\S+) (\S+) (\S+) (\S+) (.*))",
    R"((\S+) (\S+) (\d+) (\S+) ([^:]+): (.*))",
    R"((\S+) (\S+) (\S+) (.*))",
    R"((\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+?)\[(\d+)\]:\s+(.*))",
    R"((\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)(?:\[(\d+)\])?:\s+(.*))",
};



bool containsApacheLogs(const std::vector<std::string>& logs)
{
    static const std::regex apachePattern(
        R"(^\S+ \S+ \S+ \[[^\]]+\] "?(?:GET|POST|PUT|DELETE|HEAD|OPTIONS|PATCH) \S+ HTTP/\d(?:\.\d)?"? \d{3} \S+.*$)"
    );
    for (const auto& line : logs) {

        if (std::regex_match(line, apachePattern)) {
            return true;  // Apache-like log detected
        }
    }

    return false;
}


std::pair<Templates, int> autoParserGenerator(
    std::vector<std::string> sentences,
    std::vector<std::string> templatePaths,
    std::vector<const char*> regexs
){

    if (containsApacheLogs(sentences)) {
        Templates template_apache("");
        return std::make_pair(template_apache, -1);
    }

    int idx = -1;
    for (size_t i = 0; i < templatePaths.size(); i++) {
        // Initiliaze candidate
        Templates* templates = new Templates(templatePaths[i]);
        MatchTree* tree = new MatchTree(templates);

        // Process format and parse logs
        std::regex log_regex(regexs[i]);
        std::smatch match_results;
        std::vector<std::string> sentences_aux(sentences.size());
        for (size_t j = 0; j < sentences.size(); j++) {
            std::regex_search(sentences[j], match_results, log_regex);
            sentences_aux[j] = match_results[match_results.size() - 1].str();
        }
        ParsedMessages* parsed_logs = tree->match_batch(sentences_aux, 1);

        // Count template missmatchess
        int not_found = 0;

        for (size_t j = 0; j < sentences.size(); j++) {
            ParsedElement elem = parsed_logs->getElem(i);
            if (elem.log_template == "template not found" || sentences_aux[j] == "") {
                not_found += 1;
            }
        }

        // Make decision
        if (not_found == 0) {
            idx = i;
            break;
        }

    }
    if (idx == -1) {
        Templates templates("");
        return std::make_pair(templates, idx);
    }

    Templates templates(templatePaths[idx]);
    return std::make_pair(templates, idx);
}


void throwException(std::string name) {

    std::string msg = "Error: " + name + " not part of logTypes: ";
    for (std::string logType : logTypes) {
        msg = msg + logType + ", ";
    }
    throw std::runtime_error(msg);

}


std::pair<Templates, int> getTemplates(
    std::string logType, std::vector<std::string> pathTemplates
) {
    int z = 0;
    for (std::string name : logTypes) {
        if (name == logType) {
            return std::make_pair(Templates(pathTemplates[z]), z);
        }
        z++;
    }
    throwException(logType);
    return std::make_pair(Templates(""), 0);
}


std::pair<Templates, int> doAutoParse(
    std::vector<std::string> sentences,
    std::string pathFolder,
    std::string logType
) {
    std::vector<std::string> pathsCopy(pathTemplates);

    for (std::string& path : pathsCopy) {
        path = pathFolder + path;
    }


    if (logType != UNASSIGNED) {
        return getTemplates(logType, pathsCopy);
    }

    return autoParserGenerator(
        sentences, pathsCopy, regexs_patterns
    );
}
