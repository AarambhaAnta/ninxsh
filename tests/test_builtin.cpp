#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

#include "builtin.hpp"

bool test_builtin_commands() {
    bool allTestsPassed = true;

    // Test 1: Checking if 'cd' is recognized as a builtin
    {
        if (!isBuiltin("cd")) {
            std::cerr << "Failed to recognize 'cd' as a builtin" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 2: Checking if 'exit' is recognized as a builtin
    {
        if (!isBuiltin("exit")) {
            std::cerr << "Failed to recognize 'exit' as a builtin" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 3: Checking if 'clear' is recognized as a builtin
    {
        if (!isBuiltin("clear")) {
            std::cerr << "Failed to recognize 'clear' as a builtin" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 4: Checking if 'ls' is NOT recognized as a builtin
    {
        if (isBuiltin("ls")) {
            std::cerr << "Incorrectly recognized 'ls' as a builtin" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 5: Testing 'cd' command functionality
    {
        // Save current directory
        char originalDir[1024];
        if (getcwd(originalDir, sizeof(originalDir)) == nullptr) {
            std::cerr << "Failed to get current directory" << std::endl;
            allTestsPassed = false;
        } else {
            // Create test vector for 'cd /Users'
            std::vector<char*> args;
            args.push_back(strdup("cd"));
            args.push_back(strdup("/Users"));
            args.push_back(nullptr);

            // Execute builtin cd command
            executeBuiltin(args);

            // Check if directory changed
            char newDir[1024];
            if (getcwd(newDir, sizeof(newDir)) == nullptr) {
                std::cerr << "Failed to get new directory" << std::endl;
                allTestsPassed = false;
            } else if (std::string(newDir) != "/Users") {
                std::cerr << "cd command didn't change directory correctly" << std::endl;
                std::cerr << "  Expected: /Users" << std::endl;
                std::cerr << "  Got: " << newDir << std::endl;
                allTestsPassed = false;
            }

            // Change back to original directory
            chdir(originalDir);

            // Free memory
            free(args[0]);
            free(args[1]);
        }
    }

    // Test 6: Testing 'cd' with HOME directory
    {
        // Save current directory
        char originalDir[1024];
        if (getcwd(originalDir, sizeof(originalDir)) == nullptr) {
            std::cerr << "Failed to get current directory" << std::endl;
            allTestsPassed = false;
        } else {
            // Get HOME directory
            std::string homeDir = getenv("HOME") ? getenv("HOME") : "";
            if (homeDir.empty()) {
                std::cerr << "HOME environment variable not set, skipping test" << std::endl;
            } else {
                // Create test vector for 'cd' (without args, should go to HOME)
                std::vector<char*> args;
                args.push_back(strdup("cd"));
                args.push_back(nullptr);

                // Execute builtin cd command
                executeBuiltin(args);

                // Check if directory changed to HOME
                char newDir[1024];
                if (getcwd(newDir, sizeof(newDir)) == nullptr) {
                    std::cerr << "Failed to get new directory" << std::endl;
                    allTestsPassed = false;
                } else if (std::string(newDir) != homeDir) {
                    std::cerr << "cd command without args didn't change to HOME directory"
                              << std::endl;
                    allTestsPassed = false;
                }

                // Change back to original directory
                chdir(originalDir);

                // Free memory
                free(args[0]);
            }
        }
    }

    // Test for new job control builtins
    {
        if (!isBuiltin("jobs")) {
            std::cerr << "Failed to recognize 'jobs' as a builtin" << std::endl;
            allTestsPassed = false;
        }
        if (!isBuiltin("kill")) {
            std::cerr << "Failed to recognize 'kill' as a builtin" << std::endl;
            allTestsPassed = false;
        }
        if (!isBuiltin("fg")) {
            std::cerr << "Failed to recognize 'fg' as a builtin" << std::endl;
            allTestsPassed = false;
        }
        if (!isBuiltin("bg")) {
            std::cerr << "Failed to recognize 'bg' as a builtin" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test for history builtin
    {
        if (!isBuiltin("history")) {
            std::cerr << "Failed to recognize 'history' as a builtin" << std::endl;
            allTestsPassed = false;
        }
    }

    return allTestsPassed;
}
