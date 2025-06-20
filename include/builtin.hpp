#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include <string>
#include <vector>

bool executeBuiltin(const std::vector<char*>& argv);
bool executeBuiltin(const std::vector<const char*>& argv);  // Overload for const char*
bool isBuiltin(const std::string& cmd);

#endif  // BUILTIN_HPP
