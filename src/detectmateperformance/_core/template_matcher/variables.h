#ifndef VAR_H
#define VAR_H

#include <string>
#include <deque>
#include <vector>
#include <utility>

#include "../aux.h"

class Variables {
private:
    std::deque<std::pair<size_t, size_t>> slots;  // finalized [begin,end) spans; (0,0) = zero-width
    std::pair<size_t, size_t> current;
    bool open;
    bool capture_vars;
public:
    Variables();

    Variables(bool get_variables);

    ~Variables();

    void extend(const Token& token);

    void extend_span(const Token& first, const Token& last);

    void close();

    void close_or_empty();

    void init_list();

    std::vector<std::string> export_variables(const std::string& original) const;
};

#endif
