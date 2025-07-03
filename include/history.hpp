#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <string>
#include <vector>

class History {
private:
    static const int DEFAULT_HISTORY_SIZE = 1000;
    std::vector<std::string> commands;
    std::string historyFilePath;
    size_t maxSize;

    void truncateIfNeeded();

public:
    History(size_t size = DEFAULT_HISTORY_SIZE);
    History(const std::string& filePath, size_t size = DEFAULT_HISTORY_SIZE);

    // Add a command to history
    void addCommand(const std::string& command);

    // Get all commands in history
    const std::vector<std::string>& getCommands() const;

    // Get a specific command by index
    std::string getCommand(size_t index) const;

    // Get the number of commands in history
    size_t size() const;

    // Load history from file
    bool loadFromFile();

    // Save history to file
    bool saveToFile() const;

    // Set the history file path
    void setHistoryFilePath(const std::string& filePath);

    // Get the history file path
    std::string getHistoryFilePath() const;
};

#endif  // HISTORY_HPP
