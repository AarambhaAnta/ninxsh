#include "command.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "limits.hpp"
#include "utils.hpp"

ParsedCommand::~ParsedCommand() {
    for (auto& command : pipeline) {
        for (auto arg : command.args) {
            free(arg);
        }
    }
}

// Token with context about how it was quoted
struct TokenInfo {
    std::string value;
    bool fromSingleQuotes;
    bool fromDoubleQuotes;

    TokenInfo(const std::string& val, bool singleQ = false, bool doubleQ = false)
        : value(val), fromSingleQuotes(singleQ), fromDoubleQuotes(doubleQ) {}
};

// Enhanced tokenizer that handles quotes and escapes and preserves quote context
std::vector<TokenInfo> tokenizeWithQuotesDetailed(const std::string& input) {
    std::vector<TokenInfo> tokens;
    std::string currentToken;
    bool inDoubleQuotes = false;
    bool inSingleQuotes = false;
    bool escaped = false;
    bool tokenFromSingleQuotes = false;
    bool tokenFromDoubleQuotes = false;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (escaped) {
            // Handle escaped character
            currentToken += c;
            escaped = false;
            continue;
        }

        if (c == '\\' && !inSingleQuotes) {
            // Escape next character (except in single quotes)
            escaped = true;
            continue;
        }

        if (c == '"' && !inSingleQuotes) {
            // Toggle double quote state
            if (!inDoubleQuotes) {
                tokenFromDoubleQuotes = true;
            }
            inDoubleQuotes = !inDoubleQuotes;
            continue;
        }

        if (c == '\'' && !inDoubleQuotes) {
            // Toggle single quote state
            if (!inSingleQuotes) {
                tokenFromSingleQuotes = true;
            }
            inSingleQuotes = !inSingleQuotes;
            continue;
        }

        if ((c == ' ' || c == '\t') && !inDoubleQuotes && !inSingleQuotes) {
            // Whitespace outside quotes - end current token
            if (!currentToken.empty()) {
                tokens.emplace_back(currentToken, tokenFromSingleQuotes, tokenFromDoubleQuotes);
                currentToken.clear();
                tokenFromSingleQuotes = false;
                tokenFromDoubleQuotes = false;
            }
            continue;
        }

        // Regular character - add to current token
        currentToken += c;
    }

    // Add final token if exists
    if (!currentToken.empty()) {
        tokens.emplace_back(currentToken, tokenFromSingleQuotes, tokenFromDoubleQuotes);
    }

    return tokens;
}

// Split input by pipes, handling quotes properly
std::vector<std::string> splitByPipes(const std::string& input) {
    std::vector<std::string> commands;
    std::string currentCommand;
    bool inDoubleQuotes = false;
    bool inSingleQuotes = false;
    bool escaped = false;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (escaped) {
            currentCommand += c;
            escaped = false;
            continue;
        }

        if (c == '\\' && !inSingleQuotes) {
            escaped = true;
            currentCommand += c;
            continue;
        }

        if (c == '"' && !inSingleQuotes) {
            inDoubleQuotes = !inDoubleQuotes;
            currentCommand += c;
            continue;
        }

        if (c == '\'' && !inDoubleQuotes) {
            inSingleQuotes = !inSingleQuotes;
            currentCommand += c;
            continue;
        }

        if (c == '|' && !inDoubleQuotes && !inSingleQuotes) {
            // Pipe outside quotes - split here
            commands.push_back(trim(currentCommand));
            currentCommand.clear();
            continue;
        }

        currentCommand += c;
    }

    // Add final command
    if (!currentCommand.empty()) {
        commands.push_back(trim(currentCommand));
    }

    return commands;
}

ParsedCommand parseCommand(const std::string& input) {
    ParsedCommand result;

    // Early validation: reject excessively long input to prevent DoS
    if (input.length() > ninxsh::limits::MAX_INPUT_LENGTH) {
        result.hasError = true;
        result.errorMessage = "Input too long (maximum " +
                              std::to_string(ninxsh::limits::MAX_INPUT_LENGTH) + " characters)";
        return result;
    }

    // Split the input by pipe character, respecting quotes
    std::vector<std::string> pipeCommands = splitByPipes(input);

    // Process each command in the pipeline
    for (const auto& cmdStr : pipeCommands) {
        // Tokenize the command, handling quotes and escapes with context
        std::vector<TokenInfo> allTokens = tokenizeWithQuotesDetailed(cmdStr);

        if (allTokens.empty()) {
            continue;  // Skip empty commands
        }

        std::vector<std::string> cmdTokens;
        std::string infile, outfile;
        bool isBackground = false;

        // Parse tokens for redirections and arguments
        for (size_t i = 0; i < allTokens.size(); ++i) {
            const TokenInfo& tokenInfo = allTokens[i];
            const std::string& token = tokenInfo.value;

            if (token == "<" && i + 1 < allTokens.size()) {
                infile = allTokens[++i].value;  // Get next token as input file
            } else if (token == ">" && i + 1 < allTokens.size()) {
                outfile = allTokens[++i].value;  // Get next token as output file
            } else if (token == "&" && i == allTokens.size() - 1) {
                // & at the end means background
                isBackground = true;
            } else {
                // Regular argument - handle expansion based on quote context
                std::string processedToken = token;

                // Only expand environment variables if NOT from single quotes
                if (!tokenInfo.fromSingleQuotes) {
                    processedToken = expandEnvVars(processedToken);
                }

                // Always expand paths (~ expansion)
                processedToken = expandPath(processedToken);

                cmdTokens.push_back(processedToken);
            }
        }

        if (cmdTokens.empty()) {
            continue;  // Skip commands with no actual command
        }

        // Create a Command struct for this part of the pipeline
        Command cmd;
        for (const auto& token : cmdTokens) {
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
