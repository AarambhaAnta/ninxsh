#include "utils.hpp"

#include <climits>
#include <cstdlib>
#include <pwd.h>
#include <regex>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#ifdef __APPLE__
#include <sys/syslimits.h>
#endif

#include "limits.hpp"

// Define HOST_NAME_MAX if not available
#ifndef HOST_NAME_MAX
#ifdef _POSIX_HOST_NAME_MAX
#define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
#else
#define HOST_NAME_MAX 255
#endif
#endif

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

// ANSI Color codes definitions
namespace Colors {
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string DIM = "\033[2m";

// Foreground colors
const std::string BLACK = "\033[30m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string WHITE = "\033[37m";

// Bright foreground colors
const std::string BRIGHT_BLACK = "\033[90m";
const std::string BRIGHT_RED = "\033[91m";
const std::string BRIGHT_GREEN = "\033[92m";
const std::string BRIGHT_YELLOW = "\033[93m";
const std::string BRIGHT_BLUE = "\033[94m";
const std::string BRIGHT_MAGENTA = "\033[95m";
const std::string BRIGHT_CYAN = "\033[96m";
const std::string BRIGHT_WHITE = "\033[97m";
}  // namespace Colors

std::string getCurrentUsername() {
    // Try to get username from environment variable first
    const char* user = getenv("USER");
    if (user) {
        return std::string(user);
    }

    // Fall back to getpwuid if USER env var is not set
    uid_t uid = getuid();
    struct passwd* pw = getpwuid(uid);
    if (pw && pw->pw_name) {
        return std::string(pw->pw_name);
    }

    // Last resort: return "user"
    return "user";
}

std::string getCurrentHostname() {
    char hostname[HOST_NAME_MAX + 1];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        hostname[HOST_NAME_MAX] = '\0';  // Ensure null termination
        return std::string(hostname);
    }

    // Fall back to environment variable
    const char* host = getenv("HOSTNAME");
    if (host) {
        return std::string(host);
    }

    // Last resort: return "localhost"
    return "localhost";
}

std::string getCurrentWorkingDir() {
    char* cwd = getcwd(nullptr, 0);
    if (cwd) {
        std::string result(cwd);
        free(cwd);

        // Replace home directory with ~ for shorter display
        const char* home = getenv("HOME");
        if (home) {
            std::string homeStr(home);
            if (result.compare(0, homeStr.length(), homeStr) == 0) {
                if (result.length() == homeStr.length()) {
                    result = "~";
                } else if (result[homeStr.length()] == '/') {
                    result = "~" + result.substr(homeStr.length());
                }
            }
        }

        // Truncate very long paths for better display
        const size_t MAX_PATH_DISPLAY = 50;
        if (result.length() > MAX_PATH_DISPLAY) {
            result = "..." + result.substr(result.length() - MAX_PATH_DISPLAY + 3);
        }

        return result;
    }

    // Fall back to PWD environment variable
    const char* pwd = getenv("PWD");
    if (pwd) {
        std::string result(pwd);

        // Apply same home directory replacement
        const char* home = getenv("HOME");
        if (home) {
            std::string homeStr(home);
            if (result.compare(0, homeStr.length(), homeStr) == 0) {
                if (result.length() == homeStr.length()) {
                    result = "~";
                } else if (result[homeStr.length()] == '/') {
                    result = "~" + result.substr(homeStr.length());
                }
            }
        }

        return result;
    }

    // Last resort
    return "~";
}

bool isOutputToTerminal() {
    return isatty(STDOUT_FILENO) != 0;
}

std::string getColoredPrompt() {
    // Only use colors if output is to a terminal (not piped)
    if (!isOutputToTerminal()) {
        std::string username = getCurrentUsername();
        std::string hostname = getCurrentHostname();
        std::string cwd = getCurrentWorkingDir();
        return username + "@" + hostname + ":" + cwd + "$ ";
    }

    std::string username = getCurrentUsername();
    std::string hostname = getCurrentHostname();
    std::string cwd = getCurrentWorkingDir();

    // Create a colorful prompt: username@hostname:path$
    // username in green, @ in white, hostname in blue, : in white,
    // path in yellow, $ in bright white
    return Colors::BOLD + Colors::GREEN + username + Colors::WHITE + "@" + Colors::BLUE + hostname +
           Colors::WHITE + ":" + Colors::YELLOW + cwd + Colors::BRIGHT_WHITE + "$ " + Colors::RESET;
}