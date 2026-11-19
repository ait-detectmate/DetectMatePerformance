#ifndef M_AUX_H
#define M_AUX_H

#include <string>
#include <deque>


std::string clean_string(const std::string& input);

std::deque<std::string> preprocessing(std::string message);

std::deque<std::string> readFileToLines(const std::string& filename);

#endif
