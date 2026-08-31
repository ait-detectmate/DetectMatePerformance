
#include "auto_parser.h"

#include <iostream>
#include <regex>


std::vector<std::string> pathTemplates = {
    "audit_templates.txt",
    "bgl_templates.txt",
    "dnsmasq_templates.txt",
    "hdfs_templates.txt",
    "openvpn.txt",
    "syslog_templates.txt",
    "thunderbird_templates.txt",
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


std::pair<Templates, int> autoParserGenerator(
    std::vector<std::string> sentences,
    std::vector<std::string> templatePaths,
    std::vector<const char*> regexs
){

    int idx = 0;
    int min_count = sentences.size();
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
            std::cout << "Original " << sentences_aux[j] << std::endl;
        }
        ParsedMessages* parsed_logs = tree->match_batch(sentences_aux, 1);

        // Count template missmatchess
        int not_found = 0;
        for (size_t j = 0; j < sentences.size(); j++) {
            ParsedElement elem = parsed_logs->getElem(i);
            std::cout << elem.log_template << "\n";
            if (elem.log_template == "template not found" || sentences_aux[j] == "") {
                not_found += 1;
            }
        }

        // Make decision
        std::cout << i << " Not found " << not_found << " " << templatePaths[i] << "\n";
        if (not_found < min_count) {
            idx = i;
            min_count = not_found;
        }

        //if (not_found == 0) {
        //    break;
        //}

    }

    Templates templates(templatePaths[idx]);

    return std::make_pair(templates, idx);
}


std::pair<Templates, int> doAutoParse(
    std::vector<std::string> sentences,
    std::string pathFolder
) {
    std::vector<std::string> pathsCopy(pathTemplates);

    for (std::string& path : pathsCopy) {
        path = pathFolder + path;
    }

    return autoParserGenerator(
        sentences, pathsCopy, regexs_patterns
    );
}
