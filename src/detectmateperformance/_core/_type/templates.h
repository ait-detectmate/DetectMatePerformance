#ifndef M_TEMPLATES_H
#define M_TEMPLATES_H

#include <vector>
#include <utility> 
#include <string>
#include <deque>

std::vector<std::string> te_preprocess(std::vector<std::string> message); 

class Templates {
protected:
    std::deque<std::vector<std::string>> messages;
    int count;

public:
    Templates(std::deque<std::string> templates);
    Templates(std::string message);

    ~Templates();

    std::vector<std::string> getNext();
    std::string getNextConcatenate();
    int size();
    std::pair<int, int> shape();
    void resetCount();
};
#endif 