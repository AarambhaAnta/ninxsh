#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>

std::string expandPath(const std::string& path);
std::string expandEnvVars(const std::string& str);
std::vector<std::string> tokenize(const std::string& str, char delimiter);
std::string trim(const std::string& str);

// Terminal prompt utilities
std::string getCurrentUsername();
std::string getCurrentHostname();
std::string getCurrentWorkingDir();
std::string getColoredPrompt();
bool isOutputToTerminal();

// ANSI Color codes
namespace Colors {
extern const std::string RESET;
extern const std::string BOLD;
extern const std::string DIM;

// Foreground colors
extern const std::string BLACK;
extern const std::string RED;
extern const std::string GREEN;
extern const std::string YELLOW;
extern const std::string BLUE;
extern const std::string MAGENTA;
extern const std::string CYAN;
extern const std::string WHITE;

// Bright foreground colors
extern const std::string BRIGHT_BLACK;
extern const std::string BRIGHT_RED;
extern const std::string BRIGHT_GREEN;
extern const std::string BRIGHT_YELLOW;
extern const std::string BRIGHT_BLUE;
extern const std::string BRIGHT_MAGENTA;
extern const std::string BRIGHT_CYAN;
extern const std::string BRIGHT_WHITE;
}  // namespace Colors

#endif  // UTIL_HPP
