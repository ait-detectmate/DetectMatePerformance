#ifndef AECID_PG_H_
#define AECID_PG_H_

#include <map>
#include <string>
#include <vector>

// ****** Internal methods
struct Tree {
    std::map<std::string, Tree> children;
    std::map<std::string, int> count;
    bool is_end_state = false;

    bool operator==(const Tree& other) const;
};

Tree InitTree();


void AddLog(Tree& tree, const std::vector<std::string>& log);
void AddLogs(Tree& tree, const std::vector<std::string>& logs);

void MergeLeft(Tree& tree_left, const Tree& tree_right);

bool IsRule1(
    const Tree& tree,
    const std::vector<std::string>& children,
    double theta_1,
    int n
);
void ApplyRule1(Tree& tree, const std::vector<std::string>& children);

bool IsRule2(
    const Tree& tree,
    const std::string& child,
    double theta_2,
    int n
);
void ApplyRule2(Tree& tree, const std::string& child);

bool Rule3(
    Tree& tree,
    const std::vector<std::string>& children,
    double theta_1,
    double theta_3,
    int n
);


void Prune(
    Tree& tree,
    double theta_1 = 0.2,
    double theta_2 = 0.8,
    double theta_3 = 0.5
);
std::vector<std::string> GetTemplates(const Tree& tree);

// ******** Parser method
std::vector<std::string> AecidPgGenerator(
    const std::vector<std::string>& logs,
    double theta_1 = 0.2,
    double theta_2 = 0.8,
    double theta_3 = 0.5
);

#endif  // AECID_PG_H_
