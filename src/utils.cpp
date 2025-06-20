#include "utils.hpp"

#include <cstdlib>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

std::string expandPath(const std::string& path) {
    std::string result = path;

    if (!result.empty() && result[0] == '~') {
        const char* home = getenv("HOME");
        if (home) {
            result.replace(0, 1, home);
        }
    }

    std::regex envPattern(R"(\$([A-Za-z_][A-Za-z0-9_]*))");
    std::smatch match;

    while (std::regex_search(result, match, envPattern)) {
        const char* value = getenv(match[1].str().c_str());
        result.replace(match.position(0), match.length(0), value ? value : "");
    }

    return result;
}

std::string expandEnvVars(const std::string& str) {
    std::string result = str;
    std::regex envPattern(R"(\$([A-Za-z_][A-Za-z0-0_]*))");
    std::smatch match;

    while (std::regex_search(result, match, envPattern)) {
        const char* value = getenv(match[1].str().c_str());
        result.replace(match.position(0), match.length(0), value ? value : "");
    }

    return result;
}

std::vector<std::string> tokenize(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

std::string trim(const std::string& str) {
    const auto start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }

    const auto end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}