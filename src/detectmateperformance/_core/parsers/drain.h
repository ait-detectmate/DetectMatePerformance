#ifndef M_DRAIN_H
#define M_DRAIN_H

#include <vector>
#include <utility>
#include <string>
#include <deque>

#include "../_type/templates.h"


float calculate_sim(std::string sentence_1, std::string sentence_2);

std::string generate_template(std::deque<std::string> sentences);

std::deque<std::string> generate_templates(
    std::vector<std::deque<std::string>> sentences, float simSeq
);

std::deque<std::string> clean_templates(std::deque<std::string> templates);


Templates* drain_generator(
    std::vector<std::deque<std::string>> sentences, float SimSeq
);

#endif
