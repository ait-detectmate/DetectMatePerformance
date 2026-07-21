#include "drain.h"

#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>
#include <unordered_set>


/////// Support methods

std::vector<std::string> split_sentence(std::string sentence) {

    std::istringstream iss(sentence);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word) {
        words.push_back(word);
    }

    return words;
}


std::string join_sentence(std::vector<std::string> words) {
    std::ostringstream oss;

    for (size_t i = 0; i < words.size(); ++i) {
        if (i != 0) oss << " ";
        oss << words[i];
    }

    return oss.str();
}

/////// Main methods

float calculate_sim(std::string sentence_1, std::string sentence_2) {
    int n = sentence_1.size();
    if (sentence_2.size() < n)
        n = sentence_2.size();

    float sims = 0.0;
    for (int i = 0; i < n; i++) {
        if (sentence_1[i] == sentence_2[i])
            sims ++;
    }

    return sims / n;
}


std::string generate_template(std::deque<std::string> sentences) {
    if (sentences.empty()) return "";

    std::vector<std::vector<std::string>> splitSentences;
    int m = 0;
    for (size_t i = 0; i < sentences.size(); i++) {
        splitSentences.push_back(split_sentence(sentences[i]));

        if (i == 0 || splitSentences[i].size() < m){
            m = splitSentences[i].size();
        }
    }

    std::vector<std::string> words;
    for (size_t j = 0; j < m; ++j) {
        std::string aux = splitSentences[0][j];
        for (size_t i = 1; i < splitSentences.size(); ++i) {
            if (splitSentences[i][j] != aux) {
                aux = "<*>";
                break;
            }
        }
        words.push_back(aux);
    }

    return join_sentence(words);
}


std::deque<std::string> generate_templates(
    std::vector<std::deque<std::string>> sentences, float simSeq
) {

    std::deque<std::string> queueSent;
    std::deque<std::string> queueSentCopy;

    std::deque<std::string> templates;
    std::deque<std::string> similar;
    std::string template_ = "";

    for (size_t i = 0; i < sentences.size(); i++) {
        queueSent = sentences[i];

        while (queueSent.size() > 0) {
            template_ = queueSent[0];
            queueSent.pop_front();

            if (queueSent.size() == 0) {
                templates.push_back(template_);

            } else {

                similar = {template_};
                queueSentCopy = {};

                for (size_t j = 0; j < queueSent.size(); j++) {
                    if (calculate_sim(template_, queueSent[j]) > simSeq) {
                        similar.push_back(queueSent[j]);
                    } else {
                        queueSentCopy.push_back(queueSent[j]);
                    }

                }

                queueSent = queueSentCopy;
                templates.push_back(generate_template(similar));
            }
        }
    }

    return templates;
}


std::deque<std::string> clean_templates(std::deque<std::string> templates) {

    // Change "Hello <*> <*>" to "Hello <*>"
    std::regex pattern("\\s*<\\*>\\s*<\\*>\\s*");
    for (size_t i = 0; i < templates.size(); i++) {
        templates[i] = std::regex_replace(templates[i], pattern, " <*> ");
    }

    // Erase templates "<*>"
    auto it = std::find(templates.begin(), templates.end(), "<*>");
    if (it != templates.end()) {
        templates.erase(it);
    }


    // Remove duplicate templates
    std::unordered_set<std::string> seen;
    std::deque<std::string> uniqueTemplates;

    for (const auto& word : templates) {
        if (seen.find(word) == seen.end()) {
            seen.insert(word);
            uniqueTemplates.push_back(word);
        }
    }

    return uniqueTemplates;
}


Templates* drain_generator(
    std::vector<std::deque<std::string>> sentences, float SimSeq
) {

    std::deque<std::string> templates = generate_templates(sentences, SimSeq);
    templates = clean_templates(templates);

    Templates* temp_instance = new Templates(templates);

    return temp_instance;
}
