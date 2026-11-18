#include "aecid_pg.h"

#include <algorithm>
#include <functional>
#include <utility>


// *** Tree class
bool Tree::operator==(const Tree& other) const {

  return children == other.children &&
         count == other.count &&
         is_end_state == other.is_end_state;

}

Tree InitTree() { return {}; }


// ***** Operation methods with tree class
void AddLog(Tree& tree, const std::vector<std::string>& log) {

    if (log.empty()) {
      tree.is_end_state = true;
      return;
    }

    const auto& head = log.front();
    if (tree.children.find(head) != tree.children.end()) {
        ++tree.count.at(head);

    } else {
        tree.children[head] = InitTree();
        tree.count[head] = 1;
    }

    const std::vector<std::string> tail(log.begin() + 1, log.end());
    AddLog(tree.children.at(head), tail);
}

void AddLogs(Tree& tree, const std::vector<std::string>& logs) {
    for (const auto& log : logs) {
        std::vector<std::string> words;
        std::size_t start = 0;

      for (;;) {
          const auto end = log.find(' ', start);
          words.push_back(log.substr(
              start, end == std::string::npos ? std::string::npos : end - start));
          if (end == std::string::npos) {
            break;
          }
          start = end + 1;
        }
        AddLog(tree, words);
      }
}

void MergeLeft(Tree& tree_left, const Tree& tree_right) {

  tree_left.is_end_state |= tree_right.is_end_state;
  for (const auto& [child, subtree] : tree_right.children) {

      if (tree_left.children.find(child) == tree_left.children.end()) {
          tree_left.children[child] = subtree;
          tree_left.count[child] = tree_right.count.at(child);

      } else {
          tree_left.count.at(child) += tree_right.count.at(child);
          MergeLeft(tree_left.children.at(child), subtree);
      }

  }
}

bool IsRule1(
    const Tree& tree,
    const std::vector<std::string>& children,
    double theta_1,
    int n
) {

    double maximum = static_cast<double>(tree.count.at(children.at(0))) / n;

    for (const auto& child : children) {
      maximum = std::max(maximum, static_cast<double>(tree.count.at(child)) / n);
    }

    return theta_1 > maximum;
}

void ApplyRule1(Tree& tree, const std::vector<std::string>& children) {

    auto subtree = tree.children.extract(children.at(0));
    auto count = tree.count.extract(children.at(0));

    tree.children["<*>"] = std::move(subtree.mapped());
    tree.count["<*>"] = count.mapped();

    for (std::size_t i = 1; i < children.size(); ++i) {
        const auto& child = children[i];
        tree.count.at("<*>") += tree.count.at(child);

        MergeLeft(tree.children.at("<*>"), tree.children.at(child));
        tree.children.erase(child);
        tree.count.erase(child);
    }

}

bool IsRule2(
    const Tree& tree,
    const std::string& child,
    double theta_2,
    int n
) {

    return static_cast<double>(tree.count.at(child)) / n < theta_2;

}

void ApplyRule2(Tree& tree, const std::string& child) {

  if (tree.children.find("<*>") == tree.children.end()) {
      auto subtree = tree.children.extract(child);
      auto count = tree.count.extract(child);

      tree.children["<*>"] = std::move(subtree.mapped());
      tree.count["<*>"] = count.mapped();
  } else {
      tree.count.at("<*>") += tree.count.at(child);
      MergeLeft(tree.children.at("<*>"), tree.children.at(child));

      tree.children.erase(child);
      tree.count.erase(child);
  }

}

bool Rule3(
    Tree& tree,
    const std::vector<std::string>& children,
    double theta_1,
    double theta_3,
    int n
) {

    std::vector<double> proportions;
    double sum = 0.0;

    for (const auto& child : children) {
        const double proportion = static_cast<double>(tree.count.at(child)) / n;
        proportions.push_back(proportion);

        if (proportion >= theta_1) {
            sum += proportion;
        }

    }
    if (sum < theta_3) {
        return false;
    }

    std::vector<std::string> filtered_children;
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (proportions[i] < theta_1) {
            filtered_children.push_back(children[i]);
        }
    }

    if (!filtered_children.empty()) {
        ApplyRule1(tree, filtered_children);
    }
    return true;
}

void Prune(Tree& tree, double theta_1, double theta_2, double theta_3) {

  std::vector<std::string> children;
  int n = 0;

  for (const auto& entry : tree.children) {
      children.push_back(entry.first);
      n += tree.count.at(entry.first);
  }

  if (children.empty()) {
      return;
  }

  if (IsRule1(tree, children, theta_1, n)) {
      ApplyRule1(tree, children);

  } else if (!Rule3(tree, children, theta_1, theta_3, n)) {
      for (const auto& child : children) {
          if (IsRule2(tree, child, theta_2, n)) {
              ApplyRule2(tree, child);
          }
      }
  }

  for (auto& [child, subtree] : tree.children) {
      const double damp = 1.0 - static_cast<double>(tree.count.at(child)) / n;
      Prune(
        subtree,
        theta_1 * (1.0 + damp),
        theta_2 * (1.0 + damp),
        theta_3 * (1.0 + damp));
    }
}

std::vector<std::string> GetTemplates(const Tree& tree) {
    std::vector<std::string> templates;
    std::vector<std::string> words;

    std::function<void(const Tree&)> visit = [&](const Tree& node) {
        if (node.is_end_state) {
            std::string text;
            for (std::size_t i = 0; i < words.size(); ++i) {
                if (i != 0) {
                    text += ' ';
                }

                text += words[i];
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

std::vector<std::string> AecidPgGenerator(
    const std::vector<std::string>& logs,
    double theta_1,
    double theta_2,
    double theta_3
) {

    Tree tree = InitTree();
    AddLogs(tree, logs);
    Prune(tree, theta_1, theta_2, theta_3);
    return GetTemplates(tree);

}
