#ifndef AECID_PG_H_
#define AECID_PG_H_

#include <map>
#include <string>
#include <vector>

namespace aecid_pg {

struct Tree {
    std::map<std::string, Tree> children;
    std::map<std::string, int> count;
    bool is_end_state = false;

    bool operator==(const Tree& other) const;
};

struct AecidPgThresholds {
    double theta_1 = 0.2;
    double theta_2 = 0.8;
    double theta_3 = 0.5;
};

Tree initTree();

void addLog(Tree& tree, const std::vector<std::string>& log);
void addLogs(Tree& tree, const std::vector<std::string>& logs);

void mergeLeft(Tree& tree_left, const Tree& tree_right);

bool isRule1(
    const Tree& tree,
    const std::vector<std::string>& children,
    const AecidPgThresholds& thresholds,
    int n
);
void applyRule1(Tree& tree, const std::vector<std::string>& children);

bool isRule2(
    const Tree& tree,
    const std::string& child,
    const AecidPgThresholds& thresholds,
    int n
);
void applyRule2(Tree& tree, const std::string& child);

bool rule3(
    Tree& tree,
    const std::vector<std::string>& children,
    const AecidPgThresholds& thresholds,
    int n
);

void prune(Tree& tree, const AecidPgThresholds& thresholds = {});
std::vector<std::string> getTemplates(const Tree& tree);

}  // namespace aecid_pg

std::vector<std::string> aecidPgGenerator(
    const std::vector<std::string>& logs,
    double theta_1 = aecid_pg::AecidPgThresholds{}.theta_1,
    double theta_2 = aecid_pg::AecidPgThresholds{}.theta_2,
    double theta_3 = aecid_pg::AecidPgThresholds{}.theta_3
);

#endif  // AECID_PG_H_
