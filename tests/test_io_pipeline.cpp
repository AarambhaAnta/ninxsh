#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

#include "command.hpp"
#include "executor.hpp"

bool test_io_redirection() {
    bool allTestsPassed = true;

    // Test 1: Output redirection
    {
        // Create a temporary file
        std::string outputFile = "/tmp/ninxsh_io_test.txt";

        // Prepare command: echo "test output" > outputFile
        Command cmd;
        cmd.args.push_back(strdup("echo"));
        cmd.args.push_back(strdup("test output"));
        cmd.args.push_back(nullptr);
        cmd.outputFile = outputFile;
        cmd.isBackground = false;

        ParsedCommand parsed;
        parsed.pipeline.push_back(cmd);

        // Execute command
        executeExternal(parsed);

        // Check file content
        std::ifstream file(outputFile);
        std::string content;
        std::getline(file, content);
        file.close();

        // Clean up
        unlink(outputFile.c_str());

        if (content != "test output") {
            std::cerr << "Output redirection test failed" << std::endl;
            std::cerr << "  Expected: 'test output'" << std::endl;
            std::cerr << "  Got: '" << content << "'" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 2: Input redirection
    {
        // Create a temporary input file
        std::string inputFile = "/tmp/ninxsh_input_test.txt";
        std::string outputFile = "/tmp/ninxsh_output_test.txt";

        // Write test data to input file
        {
            std::ofstream file(inputFile);
            file << "line 1\nline 2\nline 3\n";
            file.close();
        }

        // Prepare command: wc -l < inputFile > outputFile
        Command cmd;
        cmd.args.push_back(strdup("wc"));
        cmd.args.push_back(strdup("-l"));
        cmd.args.push_back(nullptr);
        cmd.inputFile = inputFile;
        cmd.outputFile = outputFile;
        cmd.isBackground = false;

        ParsedCommand parsed;
        parsed.pipeline.push_back(cmd);

        // Execute command
        executeExternal(parsed);

        // Check file content (should be the count of lines, which is 3)
        std::ifstream file(outputFile);
        std::string content;
        std::getline(file, content);
        file.close();

        // Clean up
        unlink(inputFile.c_str());
        unlink(outputFile.c_str());

        // Extract the number from content (wc -l output might have spaces)
        int count = 0;
        for (char c : content) {
            if (isdigit(c)) {
                count = count * 10 + (c - '0');
            }
        }

        if (count != 3) {
            std::cerr << "Input redirection test failed" << std::endl;
            std::cerr << "  Expected line count: 3" << std::endl;
            std::cerr << "  Got: " << count << std::endl;
            allTestsPassed = false;
        }
    }

    return allTestsPassed;
}

bool test_pipeline() {
    bool allTestsPassed = true;

    // Test: Simple pipeline (ls | grep .cpp | wc -l)
    {
        std::string outputFile = "/tmp/ninxsh_pipeline_test.txt";

        // Create the pipeline commands
        Command lsCmd, grepCmd, wcCmd;

        lsCmd.args.push_back(strdup("ls"));
        lsCmd.args.push_back(strdup("-1"));
        lsCmd.args.push_back(nullptr);
        lsCmd.isBackground = false;

        grepCmd.args.push_back(strdup("grep"));
        grepCmd.args.push_back(strdup(".cpp"));
        grepCmd.args.push_back(nullptr);
        grepCmd.isBackground = false;

        wcCmd.args.push_back(strdup("wc"));
        wcCmd.args.push_back(strdup("-l"));
        wcCmd.args.push_back(nullptr);
        wcCmd.outputFile = outputFile;
        wcCmd.isBackground = false;

        ParsedCommand parsed;
        parsed.pipeline.push_back(lsCmd);
        parsed.pipeline.push_back(grepCmd);
        parsed.pipeline.push_back(wcCmd);

        // Execute the pipeline
        executeExternal(parsed);

        // Check the result (there should be a number in the output file)
        std::ifstream file(outputFile);
        std::string content;
        std::getline(file, content);
        file.close();

        // Clean up
        unlink(outputFile.c_str());

        // Check if content contains a number (doesn't matter what number)
        bool hasDigit = false;
        for (char c : content) {
            if (isdigit(c)) {
                hasDigit = true;
                break;
            }
        }

        if (!hasDigit) {
            std::cerr << "Pipeline test failed, output doesn't contain any digits" << std::endl;
            std::cerr << "  Output: '" << content << "'" << std::endl;
            allTestsPassed = false;
        }
    }

    return allTestsPassed;
}
