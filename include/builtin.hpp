#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include <string>
#include <vector>

bool isBuiltin(const std::string &cmd);
bool executeBuiltin(const std::vector<char *> &argv);

#endif // BUILTIN_HPP
