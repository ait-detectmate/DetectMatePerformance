#ifndef VAR_H
#define VAR_H

#include <vector>
#include <utility>
#include <string>
#include <deque>


class Variables {
private:
    std::deque<std::string> list;
    bool capture_vars;
public:
    Variables();

    Variables(bool get_variables);

    ~Variables();

    void add_variable(std::string variable);

    void add_variables(std::deque<std::string> variables);

    void init_list();

    std::deque<std::string> export_variables() const;

};



#endif
