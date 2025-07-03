#include "utils.hpp"

#include <cstdlib>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "limits.hpp"

// Static regex pattern for environment variable matching (compiled once)
static const std::regex envPattern(R"(\$([A-Za-z_][A-Za-z0-9_]*))");

std::string expandPath(const std::string& path) {
    // Early validation: reject excessively long paths to prevent DoS
    if (path.length() > ninxsh::limits::MAX_PATH_LENGTH) {
        return path;  // Return unchanged if too long
    }

    std::string result = path;

    // Handle tilde expansion
    if (!result.empty() && result[0] == '~') {
        const char* home = getenv("HOME");
        if (home) {
            result.replace(0, 1, home);
        }
    }

    // Quick check: if there's no '$' character, skip expensive regex processing
    if (result.find('$') == std::string::npos) {
        return result;
    }

    // Use static regex pattern (compiled once)
    std::smatch match;

    while (std::regex_search(result, match, envPattern)) {
        const char* value = getenv(match[1].str().c_str());
        result.replace(match.position(0), match.length(0), value ? value : "");
    }

    return result;
}

std::string expandEnvVars(const std::string& str) {
    // Early validation: reject excessively long strings to prevent DoS
    if (str.length() > ninxsh::limits::MAX_STRING_LENGTH) {
        return str;  // Return unchanged if too long
    }

    // Quick check: if there's no '$' character, skip expensive regex processing
    if (str.find('$') == std::string::npos) {
        return str;
    }

    std::string result = str;
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