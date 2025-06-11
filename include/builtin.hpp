#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include <vector>

bool executeBuiltin(const std::vector<char*>& argv);
bool isBuiltin(const std::string& cmd);

#endif  // BUILTIN_HPP
