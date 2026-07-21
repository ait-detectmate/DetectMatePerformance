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

    std::vector<std::vector<std::string>> split_sentences;
    int m = 0;
    for (size_t i = 0; i < sentences.size(); i++) {
        split_sentences.push_back(split_sentence(sentences[i]));

        if (i == 0 || split_sentences[i].size() < m){
            m = split_sentences[i].size();
        }
    }

    std::vector<std::string> words;
    for (size_t j = 0; j < m; ++j) {
        std::string aux = split_sentences[0][j];
        for (size_t i = 1; i < split_sentences.size(); ++i) {
            if (split_sentences[i][j] != aux) {
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

    std::deque<std::string> queue_sent;
    std::deque<std::string> queue_sent_copy;

    std::deque<std::string> templates;
    std::deque<std::string> similar;
    std::string template_ = "";

    for (size_t i = 0; i < sentences.size(); i++) {
        queue_sent = sentences[i];

        while (queue_sent.size() > 0) {
            template_ = queue_sent[0];
            queue_sent.pop_front();

            if (queue_sent.size() == 0) {
                templates.push_back(template_);

            } else {

                similar = {template_};
                queue_sent_copy = {};

                for (size_t j = 0; j < queue_sent.size(); j++) {
                    if (calculate_sim(template_, queue_sent[j]) > simSeq) {
                        similar.push_back(queue_sent[j]);
                    } else {
                        queue_sent_copy.push_back(queue_sent[j]);
                    }

                }

                queue_sent = queue_sent_copy;
                templates.push_back(generate_template(similar));
            }
        }
    }

    return templates;
}


std::deque<std::string> clean_templates(std::deque<std::string> templates) {

    auto it = std::find(templates.begin(), templates.end(), "<*>");
    if (it != templates.end()) {
        templates.erase(it);
    }

    std::regex pattern("\\s*<\\*>\\s*<\\*>\\s*");
    for (size_t i = 0; i < templates.size(); i++) {
        templates[i] = std::regex_replace(templates[i], pattern, " <*> ");
    }

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

/*
Templates process_log_clusters(
    std::vector<std::deque<std::string>> sentences, float SimSeq
) {

    std::deque<std::string> templates = generate_templates(sentences, SimSeq);


    return Templates(templates);
}*/
