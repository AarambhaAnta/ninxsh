#include <cassert>
#include <cstdio>  // for remove()
#include <fstream>
#include <iostream>
#include <string>

#include "../include/history.hpp"

void testHistoryBasic() {
    History history(10);  // Max size of 10 commands

    // Test adding commands
    history.addCommand("ls");
    history.addCommand("cd /tmp");
    history.addCommand("echo hello");

    assert(history.size() == 3);
    assert(history.getCommand(0) == "ls");
    assert(history.getCommand(1) == "cd /tmp");
    assert(history.getCommand(2) == "echo hello");

    // Test duplicate commands (consecutive)
    history.addCommand("echo hello");  // Should not be added (duplicate)
    assert(history.size() == 3);

    // Test max size
    for (int i = 0; i < 10; i++) {
        history.addCommand("cmd" + std::to_string(i));
    }

    // Should have truncated, keeping only the most recent 10
    assert(history.size() == 10);
    // The oldest commands are removed, so the first command should be "cmd0" now
    assert(history.getCommand(0) == "cmd0");  // Oldest remaining command

    std::cout << "Basic history functionality tests passed!\n";
}

// Mock Shell class just for testing history expansion
class MockShell {
private:
    History history;

public:
    MockShell() {
        // Initialize with some commands
        history.addCommand("ls -l");
        history.addCommand("echo hello");
        history.addCommand("cd /tmp");
        history.addCommand("pwd");
    }

    std::string expandHistoryCommand(const std::string& input) const {
        // If input is empty, no expansion needed
        if (input.empty()) {
            return input;
        }

        // Get all commands from history
        const auto& commands = history.getCommands();

        // Check for history expansions
        if (input == "!!") {
            // Execute the last command
            if (commands.empty()) {
                return "";
            }
            return commands.back();
        } else if (input[0] == '!') {
            // Check for !n format (execute command number n)
            std::string numberStr = input.substr(1);
            try {
                int cmdNumber = std::stoi(numberStr);
                // History is 1-indexed in display but 0-indexed in storage
                size_t index = cmdNumber - 1;

                if (index >= commands.size()) {
                    return "";
                }

                return commands[index];
            } catch (const std::exception& e) {
                // If not a valid number, treat as a regular command
                return input;
            }
        }

        // No expansion needed
        return input;
    }

    const History& getHistory() const {
        return history;
    }
};

void testHistoryExpansion() {
    MockShell shell;

    // Test !!
    assert(shell.expandHistoryCommand("!!") == "pwd");

    // Test !n
    assert(shell.expandHistoryCommand("!1") == "ls -l");
    assert(shell.expandHistoryCommand("!2") == "echo hello");
    assert(shell.expandHistoryCommand("!3") == "cd /tmp");
    assert(shell.expandHistoryCommand("!4") == "pwd");

    // Test invalid !n (out of range)
    assert(shell.expandHistoryCommand("!5") == "");

    // Test invalid !n (not a number)
    assert(shell.expandHistoryCommand("!abc") == "!abc");

    // Test regular command (no expansion)
    assert(shell.expandHistoryCommand("ls") == "ls");

    std::cout << "History expansion tests passed!\n";
}

void testHistoryFile() {
    // Create a temporary file for testing
    std::string testFile = "/tmp/ninxsh_history_test";

    // Create initial history and save to file
    {
        History history(testFile, 5);
        history.addCommand("cmd1");
        history.addCommand("cmd2");
        history.addCommand("cmd3");

        bool saved = history.saveToFile();
        assert(saved);
    }

    // Load from file and verify
    {
        History history(testFile, 5);
        bool loaded = history.loadFromFile();
        assert(loaded);

        assert(history.size() == 3);
        assert(history.getCommand(0) == "cmd1");
        assert(history.getCommand(1) == "cmd2");
        assert(history.getCommand(2) == "cmd3");

        // Add more commands and save again
        history.addCommand("cmd4");
        history.addCommand("cmd5");
        history.addCommand("cmd6");  // This should push out cmd1

        history.saveToFile();
    }

    // Verify the updated file
    {
        History history(testFile, 5);
        history.loadFromFile();

        assert(history.size() == 5);
        assert(history.getCommand(0) == "cmd2");
        assert(history.getCommand(4) == "cmd6");
    }

    // Clean up the test file
    remove(testFile.c_str());

    std::cout << "History file functionality tests passed!\n";
}

void runHistoryTests() {
    testHistoryBasic();
    testHistoryFile();
    testHistoryExpansion();

    std::cout << "All history tests passed!\n";
}
