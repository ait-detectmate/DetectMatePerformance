#include "drain.h"

#include <utility>
#include <string>
#include <vector>
#include <sstream>


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


std::string generate_template(std::vector<std::string> sentences) {
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
