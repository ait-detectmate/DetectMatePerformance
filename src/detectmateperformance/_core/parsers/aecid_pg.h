#ifndef AECID_PG_H_
#define AECID_PG_H_

#include <map>
#include <string>
#include <vector>

struct AecidPgTree {
    std::map<std::string, AecidPgTree> children;
    std::map<std::string, int> count;
    bool is_end_state = false;

    bool operator==(const AecidPgTree& other) const;
};

struct AecidPgThresholds {
    double theta_1 = 0.2;
    double theta_2 = 0.8;
    double theta_3 = 0.5;
};

AecidPgTree initTree();

void addLog(AecidPgTree& tree, const std::vector<std::string>& log);
void addLogs(AecidPgTree& tree, const std::vector<std::string>& logs);

void mergeLeft(AecidPgTree& tree_left, const AecidPgTree& tree_right);

bool isRule1(
    const AecidPgTree& tree,
    const std::vector<std::string>& children,
    const AecidPgThresholds& thresholds,
    int n
);
void applyRule1(AecidPgTree& tree, const std::vector<std::string>& children);

bool isRule2(
    const AecidPgTree& tree,
    const std::string& child,
    const AecidPgThresholds& thresholds,
    int n
);
void applyRule2(AecidPgTree& tree, const std::string& child);

bool rule3(
    AecidPgTree& tree,
    const std::vector<std::string>& children,
    const AecidPgThresholds& thresholds,
    int n
);

void prune(AecidPgTree& tree, const AecidPgThresholds& thresholds = {});
std::vector<std::string> getTemplates(const AecidPgTree& tree);

std::vector<std::string> aecidPgGenerator(
    const std::vector<std::string>& logs,
    double theta_1 = AecidPgThresholds{}.theta_1,
    double theta_2 = AecidPgThresholds{}.theta_2,
    double theta_3 = AecidPgThresholds{}.theta_3
);

#endif  // AECID_PG_H_
