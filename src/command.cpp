#include "command.hpp"
#include "builtin.hpp"
#include "utils.hpp"
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

std::vector<char*> tokenize(const std::string& input){
    std::istringstream iss(input);
    std::string word;
    std::vector<std::string> tokens;

    while(iss>>word){
        tokens.push_back(expandPath(word));
    }

    std::vector<char *> argv;
    for(const auto& token:tokens){
        argv.push_back(strdup(token.c_str()));
    }
    argv.push_back(nullptr);

    return argv;
}

bool isBuiltin(const std::string &cmd){
    return cmd == "exit" || cmd == "clear" || cmd == "cd";
}