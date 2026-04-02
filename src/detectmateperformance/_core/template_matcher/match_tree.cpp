#include <vector>
#include <utility>
#include <string>
#include <algorithm>
#include <thread>

#include "match_tree.h"
#include "tree_op.h"
#include "variables.h"
#include "../aux.h"

#include <iostream>


std::string do_match(Tree* tree_, std::string sentence, Variables* vars) {
    std::deque<std::string> sequence = preprocessing(sentence);
    std::pair<bool, Tree*> result = searchTree(tree_, sequence, vars);

    if (result.first) {
        return result.second->getTemplate();
    }

    return "template not found";
}

void do_matches(
    Tree* tree_,
    const std::vector<std::string>& sentences,
    ParsedMessages* results,
    int startIdx,
    int stopIdx
) {

    Variables* vars = new Variables(false);
    for (int i = startIdx; i < stopIdx; ++i) {
        results->setElem(i, do_match(tree_, sentences[i], vars));
    }
    delete vars;
}

void do_matches_with_var(
    Tree* tree_,
    const std::vector<std::string>& sentences,
    ParsedMessages* results,
    int startIdx,
    int stopIdx
) {

    Variables* vars = new Variables();
    for (int i = startIdx; i < stopIdx; ++i) {
        auto aux = do_match(tree_, sentences[i], vars);
        results->setElemWithVar(i, aux, vars->export_variables());
        vars->init_list();
    }
    delete vars;
}

MatchTree::MatchTree(Templates* templates) {
    this->templates = templates;
    tree = buildTree(templates);
}

MatchTree::~MatchTree() {
    delete tree;
}

ParsedMessages* MatchTree::match_string(std::string sentence) {
    Variables* vars = new Variables(false);
    std::string template_ = do_match(tree, sentence, vars);
    delete vars;

    ParsedMessages* msg = new ParsedMessages(this->templates, 1);
    msg->setElem(0, template_);

    return msg;
}

ParsedMessages* MatchTree::match_string_with_var(std::string sentence) {
    Variables* vars = new Variables();
    std::string template_ = do_match(tree, sentence, vars);

    std::string variables = vars->export_variables();
    delete vars;

    ParsedMessages* msg = new ParsedMessages(this->templates, 1);
    msg->setElemWithVar(0, template_, variables);

    return msg;
}

ParsedMessages* MatchTree::match_batch(std::vector<std::string> sentences, int n_workers) {
    int n = sentences.size();
    ParsedMessages* msg = new ParsedMessages(this->templates, n);

    if (n_workers <= 1 || n == 0) {
        do_matches(tree, sentences, msg, 0, n);

    } else {
        std::vector<std::thread> threads;
        int base = n / n_workers;
        int rem = n % n_workers;
        int start = 0;

        for (int i = 0; i < n_workers; ++i) {
            int chunk = base + (i < rem ? 1 : 0);
            int end = start + chunk;
            threads.emplace_back(
                do_matches,
                tree,
                std::cref(sentences),
                std::ref(msg),
                start,
                end
            );
            start = end;
        }

        for (std::thread& thread : threads) {
            thread.join();
        }
        threads.clear();
    }

    sentences.clear();

    return msg;

}

ParsedMessages* MatchTree::match_batch_with_var(std::vector<std::string> sentences, int n_workers){
    int n = sentences.size();
    ParsedMessages* msg = new ParsedMessages(this->templates, n);

    if (n_workers <= 1 || n == 0) {
        do_matches_with_var(tree, sentences, msg, 0, n);

    } else {
        std::vector<std::thread> threads;
        int base = n / n_workers;
        int rem = n % n_workers;
        int start = 0;

        for (int i = 0; i < n_workers; ++i) {
            int chunk = base + (i < rem ? 1 : 0);
            int end = start + chunk;
            threads.emplace_back(
                do_matches_with_var,
                tree,
                std::cref(sentences),
                std::ref(msg),
                start,
                end
            );
            start = end;
        }

        for (std::thread& thread : threads) {
            thread.join();
        }
        threads.clear();
    }

    sentences.clear();

    return msg;
}

bool MatchTree::isEqual(Tree* other_tree) {
    return tree->isEqual(other_tree);
}

bool MatchTree::isEqual(MatchTree* matcher) {
    return matcher->isEqual(tree);
}

Templates* MatchTree::getTemplates() {
    return templates;
}
