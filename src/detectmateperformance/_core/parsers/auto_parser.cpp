
#include "auto_parser.h"
#include <regex>


Templates AutoParserGenerator(
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
        }
        ParsedMessages* parsed_logs = tree->match_batch(sentences_aux, 1);

        // Count template missmatchess
        int not_found = 0;
        for (size_t j = 0; j < sentences.size(); j++) {
            ParsedElement elem = parsed_logs->getElem(i);
            if (elem.log_template == "template not found") {
                not_found += 1;
            }
        }

        // Make decision
        if (not_found < min_count) {
            idx = i;
            min_count = not_found;
        }

        if (not_found == 0) {
            break;
        }

    }

    Templates templates(templatePaths[idx]);

    return templates;
}
