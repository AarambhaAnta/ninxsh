#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

#include "command.hpp"

bool test_quote_handling() {
    bool allTestsPassed = true;

    // Test 1: Double quotes with spaces
    {
        std::string input = "echo \"hello world\"";
        ParsedCommand parsed = parseCommand(input);

        if (parsed.hasError || parsed.pipeline.size() != 1 ||
            parsed.pipeline[0].args.size() != 3) {  // echo, "hello world", nullptr
            std::cerr << "Failed double quotes test - incorrect parsing" << std::endl;
            allTestsPassed = false;
        } else {
            std::string arg1 = parsed.pipeline[0].args[1];
            if (arg1 != "hello world") {
                std::cerr << "Failed double quotes test - expected 'hello world', got '" << arg1
                          << "'" << std::endl;
                allTestsPassed = false;
            }
        }
    }

    // Test 2: Single quotes with spaces
    {
        std::string input = "echo 'hello world'";
        ParsedCommand parsed = parseCommand(input);

        if (parsed.hasError || parsed.pipeline.size() != 1 || parsed.pipeline[0].args.size() != 3) {
            std::cerr << "Failed single quotes test - incorrect parsing" << std::endl;
            allTestsPassed = false;
        } else {
            std::string arg1 = parsed.pipeline[0].args[1];
            if (arg1 != "hello world") {
                std::cerr << "Failed single quotes test - expected 'hello world', got '" << arg1
                          << "'" << std::endl;
                allTestsPassed = false;
            }
        }
    }

    // Test 3: Escape sequences
    {
        std::string input = "echo hello\\ world";
        ParsedCommand parsed = parseCommand(input);

        if (parsed.hasError || parsed.pipeline.size() != 1 || parsed.pipeline[0].args.size() != 3) {
            std::cerr << "Failed escape test - incorrect parsing" << std::endl;
            allTestsPassed = false;
        } else {
            std::string arg1 = parsed.pipeline[0].args[1];
            if (arg1 != "hello world") {
                std::cerr << "Failed escape test - expected 'hello world', got '" << arg1 << "'"
                          << std::endl;
                allTestsPassed = false;
            }
        }
    }

    // Test 4: Environment variables in double quotes (should expand)
    {
        // Set a test environment variable
        setenv("TEST_VAR", "test_value", 1);

        std::string input = "echo \"Value: $TEST_VAR\"";
        ParsedCommand parsed = parseCommand(input);

        if (parsed.hasError || parsed.pipeline.size() != 1 || parsed.pipeline[0].args.size() != 3) {
            std::cerr << "Failed env var in double quotes test - incorrect parsing" << std::endl;
            allTestsPassed = false;
        } else {
            std::string arg1 = parsed.pipeline[0].args[1];
            if (arg1 != "Value: test_value") {
                std::cerr
                    << "Failed env var in double quotes test - expected 'Value: test_value', got '"
                    << arg1 << "'" << std::endl;
                allTestsPassed = false;
            }
        }

        unsetenv("TEST_VAR");
    }

    // Test 5: Environment variables in single quotes (should NOT expand)
    {
        // Set a test environment variable
        setenv("TEST_VAR", "test_value", 1);

        std::string input = "echo 'Value: $TEST_VAR'";
        ParsedCommand parsed = parseCommand(input);

        if (parsed.hasError || parsed.pipeline.size() != 1 || parsed.pipeline[0].args.size() != 3) {
            std::cerr << "Failed env var in single quotes test - incorrect parsing" << std::endl;
            allTestsPassed = false;
        } else {
            std::string arg1 = parsed.pipeline[0].args[1];
            if (arg1 != "Value: $TEST_VAR") {
                std::cerr
                    << "Failed env var in single quotes test - expected 'Value: $TEST_VAR', got '"
                    << arg1 << "'" << std::endl;
                allTestsPassed = false;
            }
        }

        unsetenv("TEST_VAR");
    }

    // Test 6: Mixed quotes and arguments
    {
        std::string input = "echo \"double quotes\" 'single quotes' no-quotes";
        ParsedCommand parsed = parseCommand(input);

        if (parsed.hasError || parsed.pipeline.size() != 1 ||
            parsed.pipeline[0].args.size() !=
                5) {  // echo, "double quotes", "single quotes", "no-quotes", nullptr
            std::cerr << "Failed mixed quotes test - incorrect parsing" << std::endl;
            allTestsPassed = false;
        } else {
            std::string arg1 = parsed.pipeline[0].args[1];
            std::string arg2 = parsed.pipeline[0].args[2];
            std::string arg3 = parsed.pipeline[0].args[3];

            if (arg1 != "double quotes" || arg2 != "single quotes" || arg3 != "no-quotes") {
                std::cerr << "Failed mixed quotes test - got args: '" << arg1 << "', '" << arg2
                          << "', '" << arg3 << "'" << std::endl;
                allTestsPassed = false;
            }
        }
    }

    // Test 7: Quotes in pipelines
    {
        std::string input = "echo \"hello world\" | grep \"hello\"";
        ParsedCommand parsed = parseCommand(input);

        if (parsed.hasError || parsed.pipeline.size() != 2) {
            std::cerr << "Failed pipeline with quotes test - incorrect parsing" << std::endl;
            allTestsPassed = false;
        } else {
            std::string arg1_1 = parsed.pipeline[0].args[1];  // First command, second arg
            std::string arg1_2 = parsed.pipeline[1].args[1];  // Second command, second arg

            if (arg1_1 != "hello world" || arg1_2 != "hello") {
                std::cerr << "Failed pipeline with quotes test - got '" << arg1_1 << "' and '"
                          << arg1_2 << "'" << std::endl;
                allTestsPassed = false;
            }
        }
    }

    return allTestsPassed;
}
