#ifndef M_DRAIN_H
#define M_DRAIN_H

#include <vector>
#include <utility>
#include <string>
#include <deque>

#include "../_type/templates.h"


float calculateSim(std::string sentence_1, std::string sentence_2);

std::string generateTemplate(std::deque<std::string> sentences);

std::deque<std::string> generateTemplates(
    std::vector<std::deque<std::string>> sentences, float simSeq
);

std::deque<std::string> cleanTemplates(std::deque<std::string> templates);


Templates drainGenerator(
    std::vector<std::deque<std::string>> sentences, float SimSeq
);

#endif
