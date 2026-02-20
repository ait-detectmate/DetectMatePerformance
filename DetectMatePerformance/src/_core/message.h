#ifndef M_MESSAGE_H
#define M_MESSAGE_H

#include <vector>
#include <utility> 
#include <string>
#include <deque>

std::deque<std::string> preprocessing(std::string message);

class Messages {
private:
    std::deque<std::deque<std::string>> messages;

public:
    Messages(std::deque<std::string> messages);
    Messages(std::string message);

    ~Messages();

    std::deque<std::string> getNextMessage();
    int size();
};
#endif 