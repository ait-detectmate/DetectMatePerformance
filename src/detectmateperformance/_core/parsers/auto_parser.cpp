
#include "auto_parser.h"


Templates AutoParserGenerator(
    std::vector<std::string> sentences,
    std::vector<std::string> templatePaths,
    std::vector<std::string> regexs
){

    for (size_t i = 0; i < templatePaths.size(); i++) {
        Templates* templates = new Templates(templatePaths[i]);
        MatchTree* tree = new MatchTree(templates);
        ParsedMessages* parsed_logs = tree->match_batch(sentences, 1);

        int not_found = 0;
        for (size_t j = 0; j < sentences.size(); j++) {
            ParsedElement elem = parsed_logs->getElem(i);
            if (elem.log_template == "template not found") {
                not_found += 1;
            }
        }

    }



    Templates templates(templatePaths[2]);

    return templates;
}
