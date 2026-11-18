#include "aecid_pg.h"

#include <algorithm>
#include <cstddef>
#include <functional>
#include <utility>


namespace aecid_pg {

bool Tree::operator==(const Tree& other) const {
    return children == other.children &&
           count == other.count &&
           is_end_state == other.is_end_state;
}

Tree initTree() {
    return {};
}

void addLog(Tree& tree, const std::vector<std::string>& log) {
    if (log.empty()) {
        tree.is_end_state = true;
        return;
    }

    const auto& head = log.front();
    if (tree.children.find(head) != tree.children.end()) {
        ++tree.count.at(head);
    } else {
        tree.children[head] = initTree();
        tree.count[head] = 1;
    }

    const std::vector<std::string> tail(log.begin() + 1, log.end());
    addLog(tree.children.at(head), tail);
}

void addLogs(Tree& tree, const std::vector<std::string>& logs) {
    std::vector<std::string> words;
    std::size_t start = 0;
    std::size_t end = 0;

    for (const auto& log : logs) {
        words.clear();
        start = 0;
        end = log.find(' ', start);
        while (end != std::string::npos) {
            words.emplace_back(log, start, end - start);
            start = end + 1;
            end = log.find(' ', start);
        }
        words.emplace_back(log, start);
        addLog(tree, words);
    }
}

void mergeLeft(Tree& tree_left, const Tree& tree_right) {
    tree_left.is_end_state |= tree_right.is_end_state;
    for (const auto& [child, subtree] : tree_right.children) {
        if (tree_left.children.find(child) == tree_left.children.end()) {
            tree_left.children[child] = subtree;
            tree_left.count[child] = tree_right.count.at(child);
        } else {
            tree_left.count.at(child) += tree_right.count.at(child);
            mergeLeft(tree_left.children.at(child), subtree);
        }
    }
}

bool isRule1(
    const Tree& tree,
    const std::vector<std::string>& children,
    const AecidPgThresholds& thresholds,
    int n
) {
    double maximum = static_cast<double>(tree.count.at(children.at(0))) / n;
    for (const auto& child : children) {
        maximum = std::max(
            maximum,
            static_cast<double>(tree.count.at(child)) / n
        );
    }

    return thresholds.theta_1 > maximum;
}

void applyRule1(Tree& tree, const std::vector<std::string>& children) {
    auto subtree = tree.children.extract(children.at(0));
    auto count = tree.count.extract(children.at(0));

    tree.children["<*>"] = std::move(subtree.mapped());
    tree.count["<*>"] = count.mapped();

    auto child = children.cbegin() + 1;
    for (; child != children.cend(); ++child) {
        tree.count.at("<*>") += tree.count.at(*child);
        mergeLeft(tree.children.at("<*>"), tree.children.at(*child));
        tree.children.erase(*child);
        tree.count.erase(*child);
    }
}

bool isRule2(
    const Tree& tree,
    const std::string& child,
    const AecidPgThresholds& thresholds,
    int n
) {
    return static_cast<double>(tree.count.at(child)) / n < thresholds.theta_2;
}

void applyRule2(Tree& tree, const std::string& child) {
    if (tree.children.find("<*>") == tree.children.end()) {
        auto subtree = tree.children.extract(child);
        auto count = tree.count.extract(child);

        tree.children["<*>"] = std::move(subtree.mapped());
        tree.count["<*>"] = count.mapped();
    } else {
        tree.count.at("<*>") += tree.count.at(child);
        mergeLeft(tree.children.at("<*>"), tree.children.at(child));
        tree.children.erase(child);
        tree.count.erase(child);
    }
}

bool rule3(
    Tree& tree,
    const std::vector<std::string>& children,
    const AecidPgThresholds& thresholds,
    int n
) {
    std::vector<double> proportions;
    double sum = 0.0;
    double proportion = 0.0;

    for (const auto& child : children) {
        proportion = static_cast<double>(tree.count.at(child)) / n;
        proportions.push_back(proportion);

        if (proportion >= thresholds.theta_1) {
            sum += proportion;
        }
    }
    if (sum < thresholds.theta_3) {
        return false;
    }

    std::vector<std::string> filtered_children;
    std::size_t index = 0;
    for (; index < children.size(); ++index) {
        if (proportions[index] < thresholds.theta_1) {
            filtered_children.push_back(children[index]);
        }
    }

    if (!filtered_children.empty()) {
        applyRule1(tree, filtered_children);
    }
    return true;
}

void prune(Tree& tree, const AecidPgThresholds& thresholds) {
    std::vector<std::string> children;
    int n = 0;

    for (const auto& entry : tree.children) {
        children.push_back(entry.first);
        n += tree.count.at(entry.first);
    }

    if (children.empty()) {
        return;
    }

    if (isRule1(tree, children, thresholds, n)) {
        applyRule1(tree, children);
    } else if (!rule3(tree, children, thresholds, n)) {
        for (const auto& child : children) {
            if (isRule2(tree, child, thresholds, n)) {
                applyRule2(tree, child);
            }
        }
    }

    AecidPgThresholds child_thresholds;
    double scale = 1.0;
    for (auto& [child, subtree] : tree.children) {
        scale = 1.0 + (1.0 - static_cast<double>(tree.count.at(child)) / n);
        child_thresholds.theta_1 = thresholds.theta_1 * scale;
        child_thresholds.theta_2 = thresholds.theta_2 * scale;
        child_thresholds.theta_3 = thresholds.theta_3 * scale;
        prune(subtree, child_thresholds);
    }
}

std::vector<std::string> getTemplates(const Tree& tree) {
    std::vector<std::string> templates;
    std::vector<std::string> words;

    std::function<void(const Tree&)> visit = [&](const Tree& node) {
        if (node.is_end_state) {
            std::string text;
            std::size_t index = 0;
            for (; index < words.size(); ++index) {
                if (index != 0) {
                    text += ' ';
                }
                text += words[index];
            }
            templates.push_back(std::move(text));
        }

        for (const auto& [token, child] : node.children) {
            words.push_back(token);
            visit(child);
            words.pop_back();
        }
    };

    visit(tree);
    return templates;
}

}  // namespace aecid_pg

std::vector<std::string> aecidPgGenerator(
    const std::vector<std::string>& logs,
    double theta_1,
    double theta_2,
    double theta_3
) {
    const aecid_pg::AecidPgThresholds thresholds{theta_1, theta_2, theta_3};
    aecid_pg::Tree tree = aecid_pg::initTree();
    aecid_pg::addLogs(tree, logs);
    aecid_pg::prune(tree, thresholds);
    return aecid_pg::getTemplates(tree);
}
