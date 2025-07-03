#include "shell.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.hpp"
#include "command.hpp"
#include "executor.hpp"

Shell::Shell() {
    // Load history from file if available
    history.loadFromFile();
}

Shell::~Shell() {
    // Save history to file when shell exits
    history.saveToFile();
}

void Shell::run() {
    setupSignalHandlers();
    std::string input;

    while (true) {
        cleanupZombieProcesses();
        printPrompt();
        std::getline(std::cin, input);

        // Handle EOF (Ctrl+D)
        if (std::cin.eof()) {
            std::cout << '\n';
            break;
        }

        // Skip empty input
        if (input.empty())
            continue;

        // Check for history expansion (!n or !!)
        std::string expandedInput = expandHistoryCommand(input);
        if (expandedInput.empty()) {
            // History expansion failed
            continue;
        }

        // If the input was changed by history expansion, echo the command
        if (expandedInput != input) {
            std::cout << expandedInput << std::endl;
            input = expandedInput;
        }

        ParsedCommand parsed = parseCommand(input);

        // Check for parsing errors
        if (parsed.hasError) {
            std::cout << "ninxsh: " << parsed.errorMessage << "\n";
            continue;
        }

        // Skip if no commands were parsed
        if (parsed.pipeline.empty() || parsed.pipeline[0].args.empty()) {
            continue;
        }

        // Add valid command to history (after validation)
        history.addCommand(input);

        // Get the first command to check if it's a builtin
        std::string cmd = parsed.pipeline[0].args[0];

        // Check for history command
        if (cmd == "history" && parsed.pipeline.size() == 1) {
            std::vector<std::string> histArgs;
            for (auto arg : parsed.pipeline[0].args) {
                if (arg) {
                    histArgs.push_back(std::string(arg));
                }
            }
            displayHistory(histArgs);
            continue;
        }

        // Only run builtins if it's a single command (not a pipeline)
        if (parsed.pipeline.size() == 1 && isBuiltin(cmd)) {
            executeBuiltin(parsed.pipeline[0].args);
            continue;
        }

        executeExternal(parsed);
    }
}

void Shell::printPrompt() const {
    std::cout << "🔮ninxsh > " << std::flush;
}

std::string Shell::expandHistoryCommand(const std::string& input) const {
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
            std::cout << "ninxsh: !!: event not found\n";
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
                std::cout << "ninxsh: " << input << ": event not found\n";
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

void Shell::displayHistory(const std::vector<std::string>& args) const {
    // Get the commands from history
    const auto& commands = history.getCommands();

    // Determine how many commands to display
    size_t numToDisplay = commands.size();

    // If an argument is provided, use it to limit the number of commands displayed
    if (args.size() > 1) {
        try {
            numToDisplay = std::min(numToDisplay, static_cast<size_t>(std::stoi(args[1])));
        } catch (const std::exception& e) {
            // If argument isn't a valid number, show all
        }
    }

    // Calculate the starting index
    size_t startIdx = (numToDisplay < commands.size()) ? commands.size() - numToDisplay : 0;

    // Display the commands with their indices
    for (size_t i = startIdx; i < commands.size(); ++i) {
        std::cout << (i + 1) << "  " << commands[i] << '\n';
    }
}
