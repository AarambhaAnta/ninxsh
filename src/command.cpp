#include "command.hpp"
#include "builtin.hpp"
#include "utils.hpp"
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

ParsedCommand::~ParsedCommand()
{
    for (auto arg : args)
    {
        free(arg);
    }
}

ParsedCommand parseCommand(const std::string &input)
{
    std::istringstream iss(input);
    std::string word;
    std::vector<std::string> tokens;
    std::string infile, outfile;

    while (iss >> word)
    {
        if (word == "<")
        {
            iss >> infile;
        }
        else if (word == ">")
        {
            iss >> outfile;
        }
        else
        {
            tokens.push_back(expandPath(word));
        }
    }

    ParsedCommand cmd;
    for (const auto &token : tokens)
    {
        cmd.args.push_back(strdup(token.c_str()));
    }
    cmd.args.push_back(nullptr);
    cmd.inputFile = infile;
    cmd.outputFile = outfile;
    return cmd;
}

bool isBuiltin(const std::string &cmd)
{
    return cmd == "exit" || cmd == "clear" || cmd == "cd";
}