#ifndef M_TEMPLATES_H
#define M_TEMPLATES_H

#include <vector>
#include <utility> 
#include <string>
#include <deque>
#include "message.h"

std::vector<std::string> tpreprocess(std::vector<std::string> message); 

class Templates : public Messages {
public:
    Templates(std::deque<std::string> templates);
    Templates(std::string message);

    ~Templates();

    std::string getNextConcatenate();
    int size();
    std::pair<int, int> shape();
};
#endif 