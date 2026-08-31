#ifndef M_AUX_H
#define M_AUX_H

#include <vector>
#include <utility>
#include <string>
#include <deque>
#include <algorithm>


std::deque<std::string> preprocessing(std::string message);

std::deque<std::string> readFileToLines(const std::string& filename);

#endif
