#ifndef M_DRAIN_H
#define M_DRAIN_H

#include <vector>
#include <utility>
#include <string>
#include <deque>

#include "../_type/templates.h"


float calculate_sim(std::string sentence_1, std::string sentence_2);

std::string generate_template(std::vector<std::string> sentences);

std::vector<std::string> generate_templates(
    std::vector<std::string> keys, std::vector<std::vector<std::string>> sentences
);

Templates process_log_clusters(
    std::vector<std::string> keys, std::vector<std::vector<std::string>> sentences
);

#endif
