#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

// Single Command in a pipeline
struct Command {
    std::vector<char*> args;
    std::string inputFile;
    std::string outputFile;
    bool isBackground = false;
};

struct ParsedCommand {
    // Vector of commands in a pipeline
    std::vector<Command> pipeline;

    // Error state tracking
    bool hasError = false;
    std::string errorMessage;

    ~ParsedCommand();
};

ParsedCommand parseCommand(const std::string& input);

#endif  // COMMAND_HPP
