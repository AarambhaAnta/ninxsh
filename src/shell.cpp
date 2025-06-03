#include "shell.hpp"
#include "command.hpp"
#include "builtin.hpp"
#include "executor.hpp"

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

void Shell::run()
{
    std::string input;

    while (true)
    {
        printPrompt();
        std::getline(std::cin, input);

        // Handle EOF (Ctrl + C)
        if (std::cin.eof())
        {
            std::cout << '\n';
            break;
        }

        // Skip empty input
        if (input.empty())
            continue;

        ParsedCommand parsed = parseCommand(input);

        if (parsed.args.empty())
        {
            continue;
        }

        std::string cmd = parsed.args[0];

        if (isBuiltin(cmd))
        {
            executeBuiltin(parsed.args);
            continue;
        }

        executeExternal(parsed);
    }
}

void Shell::printPrompt() const
{
    std::cout << "🔮ninxsh > " << std::flush;
}