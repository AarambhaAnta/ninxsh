#include "utils.hpp"
#include <cstdlib>
#include <regex>
#include <string>

std::string expandPath(const std::string &path){
    std::string result = path;

    if(!result.empty() && result[0] =='~'){
        const char *home = getenv("HOME");
        if(home){
            result.replace(0, 1, home);
        }
    }

    std::regex envPattern(R"(\$([A-Za-z_][A-Za-z0-9_]*))");
    std::smatch match;

    while(std::regex_search(result, match, envPattern)){
        const char *value = getenv(match[1].str().c_str());
        result.replace(match.position(0), match.length(0), value ? value : "");
    }

    return result;
}