#include "command.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "utils.hpp"

ParsedCommand::~ParsedCommand() {
    for (auto& command : pipeline) {
        for (auto arg : command.args) {
            free(arg);
        }
    }
}

ParsedCommand parseCommand(const std::string& input) {
    ParsedCommand result;

    // Early validation: reject excessively long input to prevent DoS
    const size_t MAX_INPUT_LENGTH = 4096;  // Reasonable limit for command line input
    if (input.length() > MAX_INPUT_LENGTH) {
        // Return empty result - caller should handle this as invalid input
        return result;
    }

    std::vector<std::string> pipeCommands;

    // Split the input by pipe character
    size_t start = 0;
    size_t end = input.find('|');

    while (end != std::string::npos) {
        pipeCommands.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find('|', start);
    }

    // Add the last command after the last pipe (or the entire command if no pipes)
    pipeCommands.push_back(input.substr(start));

    // Process each command in the pipeline
    for (const auto& cmdStr : pipeCommands) {
        std::istringstream iss(cmdStr);
        std::string word;
        std::vector<std::string> tokens;
        std::string infile, outfile;
        bool isBackground = false;

        // Parse each command for redirections and arguments
        while (iss >> word) {
            if (word == "<") {
                iss >> infile;
            } else if (word == ">") {
                iss >> outfile;
            } else if (word == "&" && tokens.size() > 0) {
                isBackground = true;
            } else {
                tokens.push_back(expandPath(word));
            }
        }

        // Create a Command struct for this part of the pipeline
        Command cmd;
        for (const auto& token : tokens) {
            cmd.args.push_back(strdup(token.c_str()));
        }
        cmd.args.push_back(nullptr);
        cmd.inputFile = infile;
        cmd.outputFile = outfile;
        cmd.isBackground = isBackground;

        // Add this command to the pipeline
        result.pipeline.push_back(cmd);
    }
    return result;
}
