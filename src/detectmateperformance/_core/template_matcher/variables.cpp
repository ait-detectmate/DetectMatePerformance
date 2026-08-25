#include "variables.h"

Variables::Variables() {
    this->capture_vars = true;
    this->open = false;
}

Variables::Variables(bool get_variables) {
    this->capture_vars = get_variables;
    this->open = false;
}

Variables::~Variables() {
    this->slots.clear();
}

void Variables::extend(const Token& token) {
    if (!capture_vars) return;
    if (!open) {
        current = {token.begin, token.end};
        open = true;
    } else {
        current.second = token.end;
    }
}

void Variables::extend_span(const Token& first, const Token& last) {
    if (!capture_vars) return;
    extend(first);
    current.second = last.end;
}

void Variables::close() {
    if (!capture_vars || !open) return;
    slots.push_back(current);
    open = false;
}

void Variables::close_or_empty() {
    if (!capture_vars) return;
    if (open) {
        close();
    } else {
        slots.push_back({0, 0});  // wildcard matched zero tokens: keep the slot so alignment holds
    }
}

void Variables::init_list() {
    slots.clear();
    open = false;
}

std::vector<std::string> Variables::export_variables(const std::string& original) const {
    std::vector<std::string> out;
    out.reserve(slots.size() + (open ? 1 : 0));
    for (const auto& slot : slots) {
        out.push_back(original.substr(slot.first, slot.second - slot.first));
    }
    // an unclosed slot only exists on unmatched lines; export the partial capture
    // (kept behavior: "template not found" rows are filtered downstream)
    if (open) {
        out.push_back(original.substr(current.first, current.second - current.first));
    }
    return out;
}
