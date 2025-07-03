#include <cassert>
#include <cstring>
#include <iostream>
#include <string>

#include "command.hpp"

// Helper function to compare commands
bool compareCommands(const Command& a, const Command& b) {
    if (a.args.size() != b.args.size())
        return false;
    for (size_t i = 0; i < a.args.size(); i++) {
        if (a.args[i] == nullptr && b.args[i] == nullptr) {
            continue;
        }
        if ((a.args[i] == nullptr && b.args[i] != nullptr) ||
            (a.args[i] != nullptr && b.args[i] == nullptr)) {
            return false;
        }
        if (std::string(a.args[i]) != std::string(b.args[i]))
            return false;
    }
    return a.inputFile == b.inputFile && a.outputFile == b.outputFile &&
           a.isBackground == b.isBackground;
}

bool test_command_parsing() {
    bool allTestsPassed = true;

    // Test 1: Basic command parsing
    {
        std::string input = "ls -l";
        ParsedCommand result = parseCommand(input);

        Command expected;
        expected.args.push_back(strdup("ls"));
        expected.args.push_back(strdup("-l"));
        expected.args.push_back(nullptr);
        expected.inputFile = "";
        expected.outputFile = "";
        expected.isBackground = false;

        if (result.pipeline.size() != 1 || !compareCommands(result.pipeline[0], expected)) {
            std::cerr << "Failed basic command parsing test" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 2: Input redirection
    {
        std::string input = "sort < input.txt";
        ParsedCommand result = parseCommand(input);

        Command expected;
        expected.args.push_back(strdup("sort"));
        expected.args.push_back(nullptr);
        expected.inputFile = "input.txt";
        expected.outputFile = "";
        expected.isBackground = false;

        if (result.pipeline.size() != 1 || !compareCommands(result.pipeline[0], expected)) {
            std::cerr << "Failed input redirection test" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 3: Output redirection
    {
        std::string input = "echo hello > output.txt";
        ParsedCommand result = parseCommand(input);

        Command expected;
        expected.args.push_back(strdup("echo"));
        expected.args.push_back(strdup("hello"));
        expected.args.push_back(nullptr);
        expected.inputFile = "";
        expected.outputFile = "output.txt";
        expected.isBackground = false;

        if (result.pipeline.size() != 1 || !compareCommands(result.pipeline[0], expected)) {
            std::cerr << "Failed output redirection test" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 4: Background process
    {
        std::string input = "sleep 10 &";
        ParsedCommand result = parseCommand(input);

        Command expected;
        expected.args.push_back(strdup("sleep"));
        expected.args.push_back(strdup("10"));
        expected.args.push_back(nullptr);
        expected.inputFile = "";
        expected.outputFile = "";
        expected.isBackground = true;

        if (result.pipeline.size() != 1 || !compareCommands(result.pipeline[0], expected)) {
            std::cerr << "Failed background process test" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 5: Pipeline
    {
        std::string input = "ls -l | grep txt | sort";
        ParsedCommand result = parseCommand(input);

        if (result.pipeline.size() != 3) {
            std::cerr << "Failed pipeline test: wrong number of commands" << std::endl;
            allTestsPassed = false;
        } else {
            Command expected1, expected2, expected3;

            expected1.args.push_back(strdup("ls"));
            expected1.args.push_back(strdup("-l"));
            expected1.args.push_back(nullptr);
            expected1.inputFile = "";
            expected1.outputFile = "";
            expected1.isBackground = false;

            expected2.args.push_back(strdup("grep"));
            expected2.args.push_back(strdup("txt"));
            expected2.args.push_back(nullptr);
            expected2.inputFile = "";
            expected2.outputFile = "";
            expected2.isBackground = false;

            expected3.args.push_back(strdup("sort"));
            expected3.args.push_back(nullptr);
            expected3.inputFile = "";
            expected3.outputFile = "";
            expected3.isBackground = false;

            if (!compareCommands(result.pipeline[0], expected1) ||
                !compareCommands(result.pipeline[1], expected2) ||
                !compareCommands(result.pipeline[2], expected3)) {
                std::cerr << "Failed pipeline test: command parsing incorrect" << std::endl;
                allTestsPassed = false;
            }
        }
    }

    // Test 6: Complex command with multiple features
    {
        std::string input = "grep error < log.txt | sort | uniq > errors.txt &";
        ParsedCommand result = parseCommand(input);

        if (result.pipeline.size() != 3) {
            std::cerr << "Failed complex command test: wrong number of commands" << std::endl;
            allTestsPassed = false;
        } else {
            Command expected1, expected2, expected3;

            expected1.args.push_back(strdup("grep"));
            expected1.args.push_back(strdup("error"));
            expected1.args.push_back(nullptr);
            expected1.inputFile = "log.txt";
            expected1.outputFile = "";
            expected1.isBackground = false;

            expected2.args.push_back(strdup("sort"));
            expected2.args.push_back(nullptr);
            expected2.inputFile = "";
            expected2.outputFile = "";
            expected2.isBackground = false;

            expected3.args.push_back(strdup("uniq"));
            expected3.args.push_back(nullptr);
            expected3.inputFile = "";
            expected3.outputFile = "errors.txt";
            expected3.isBackground = true;

            if (!compareCommands(result.pipeline[0], expected1) ||
                !compareCommands(result.pipeline[1], expected2) ||
                !compareCommands(result.pipeline[2], expected3)) {
                std::cerr << "Failed complex command test: command parsing incorrect" << std::endl;
                allTestsPassed = false;
            }
        }
    }

    return allTestsPassed;
}
