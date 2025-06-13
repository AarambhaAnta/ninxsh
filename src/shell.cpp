#include "shell.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.hpp"
#include "command.hpp"
#include "executor.hpp"

void Shell::run() {
    setupSignalHandlers();
    std::string input;

    while (true) {
        cleanupZombieProcesses();
        printPrompt();
        std::getline(std::cin, input);

        // Handle EOF (Ctrl + C)
        if (std::cin.eof()) {
            std::cout << '\n';
            break;
        }

        // Skip empty input
        if (input.empty())
            continue;

        ParsedCommand parsed = parseCommand(input);

        // Skip if no commands were parsed
        if (parsed.pipeline.empty() || parsed.pipeline[0].args.empty()) {
            continue;
        }

        // Get the first command to check if it's a builtin
        std::string cmd = parsed.pipeline[0].args[0];

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
