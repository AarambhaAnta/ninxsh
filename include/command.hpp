#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

struct ParsedCommand
{
    std::vector<char *> args;
    std::string inputFile;
    std::string outputFile;

    ~ParsedCommand();
};


ParsedCommand parseCommand(const std::string &input);

bool isBuiltin(const std::string &cmd);

#endif    // COMMAND_HPP