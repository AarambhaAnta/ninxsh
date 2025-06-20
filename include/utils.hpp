#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>

std::string expandPath(const std::string& path);
std::string expandEnvVars(const std::string& str);
std::vector<std::string> tokenize(const std::string& str, char delimiter);
std::string trim(const std::string& str);

#endif  // UTIL_HPP
