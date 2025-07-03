#include <cassert>
#include <chrono>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

#include "command.hpp"
#include "executor.hpp"

bool test_executor() {
    bool allTestsPassed = true;

    // Test 1: Execute a simple command and check output
    {
        // Create a temporary file for output
        std::string outputFile = "/tmp/ninxsh_test_output.txt";

        // Create a test command to write date to the file
        Command dateCmd;
        dateCmd.args.push_back(strdup("date"));
        dateCmd.args.push_back(nullptr);
        dateCmd.outputFile = outputFile;
        dateCmd.isBackground = false;

        ParsedCommand parsed;
        parsed.pipeline.push_back(dateCmd);

        // Execute the command
        executeExternal(parsed);

        // Check if the file was created and contains output
        std::ifstream file(outputFile);
        bool fileExists = file.good();
        std::string content;
        std::getline(file, content);
        file.close();

        // Clean up
        unlink(outputFile.c_str());

        if (!fileExists || content.empty()) {
            std::cerr << "Failed to execute simple command with redirection" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 2: Test background execution
    {
        // Create a test command that runs in the background
        Command sleepCmd;
        sleepCmd.args.push_back(strdup("sleep"));
        sleepCmd.args.push_back(strdup("1"));
        sleepCmd.args.push_back(nullptr);
        sleepCmd.isBackground = true;

        ParsedCommand parsed;
        parsed.pipeline.push_back(sleepCmd);

        // Execute the command and measure time
        auto start = std::chrono::steady_clock::now();
        executeExternal(parsed);
        auto end = std::chrono::steady_clock::now();

        // Calculate elapsed time
        std::chrono::duration<double> elapsed = end - start;

        // If it's a background process, it shouldn't take ~1 second
        if (elapsed.count() >= 0.9) {
            std::cerr << "Background process execution seems to be blocking" << std::endl;
            allTestsPassed = false;
        }

        // Allow background process to finish
        usleep(1100000);  // 1.1 seconds
    }

    return allTestsPassed;
}
