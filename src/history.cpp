#include "history.hpp"

#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

History::History(size_t size) : maxSize(size) {
    // By default, set the history file path to ~/.ninxsh_history
    const char* homeDir = getenv("HOME");
    if (homeDir) {
        historyFilePath = std::string(homeDir) + "/.ninxsh_history";
    }
}

History::History(const std::string& filePath, size_t size)
    : historyFilePath(filePath), maxSize(size) {}

void History::addCommand(const std::string& command) {
    // Don't add empty commands or duplicates of the last command
    if (command.empty() || (!commands.empty() && commands.back() == command)) {
        return;
    }

    commands.push_back(command);
    truncateIfNeeded();
}

const std::vector<std::string>& History::getCommands() const {
    return commands;
}

std::string History::getCommand(size_t index) const {
    if (index < commands.size()) {
        return commands[index];
    }
    return "";
}

size_t History::size() const {
    return commands.size();
}

bool History::loadFromFile() {
    if (historyFilePath.empty()) {
        return false;
    }

    std::ifstream file(historyFilePath);
    if (!file.is_open()) {
        return false;
    }

    commands.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            commands.push_back(line);
        }
    }

    file.close();
    truncateIfNeeded();
    return true;
}

bool History::saveToFile() const {
    if (historyFilePath.empty()) {
        return false;
    }

    std::ofstream file(historyFilePath);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& cmd : commands) {
        file << cmd << '\n';
    }

    file.close();
    return true;
}

void History::setHistoryFilePath(const std::string& filePath) {
    historyFilePath = filePath;
}

std::string History::getHistoryFilePath() const {
    return historyFilePath;
}

void History::truncateIfNeeded() {
    if (commands.size() > maxSize) {
        // Remove oldest commands to maintain the max size
        commands.erase(commands.begin(), commands.begin() + (commands.size() - maxSize));
    }
}
