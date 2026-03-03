#ifndef M_MESSAGE_H
#define M_MESSAGE_H

#include <vector>
#include <utility> 
#include <string>
#include <deque>

std::deque<std::string> preprocessing(std::string message);

class Messages {
protected:
    std::deque<std::deque<std::string>> messages;
    int count;
 
public:
    Messages(std::deque<std::string> messages);
    Messages(std::string message);

    ~Messages();

    std::deque<std::string> getNext();
    std::string getNextConcatenate();
    int size();
    std::pair<int, int> shape();
    void resetCount();
};
#endif 