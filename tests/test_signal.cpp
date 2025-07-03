#include <cassert>
#include <csignal>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "executor.hpp"

// Signal handling is hard to test programmatically, but we can check if our
// handlers are properly set up and if zombie processes are cleaned up

bool test_signal_handling() {
    bool allTestsPassed = true;

    // Test 1: Check if isShellForeground is initialized correctly
    {
        // When the shell starts, isShellForeground should be true
        if (!isShellForeground) {
            std::cerr << "isShellForeground is not initialized to true" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 2: Check zombie process cleanup
    {
        // Create a test temporary file
        std::string flagFile = "/tmp/ninxsh_zombie_test.txt";

        // Create a child process that will exit immediately
        pid_t child = fork();

        if (child == 0) {
            // Child process
            std::ofstream file(flagFile);
            file << "Child process executed" << std::endl;
            file.close();
            exit(0);
        } else if (child > 0) {
            // Parent process
            // Sleep a bit to let the child process exit
            usleep(100000);  // 0.1 seconds

            // Call the cleanup function
            cleanupZombieProcesses();

            // Try to waitpid on the child to see if it was already reaped
            int status;
            pid_t result = waitpid(child, &status, WNOHANG);

            // If result is -1, the child wasn't found (already reaped)
            // If result is 0, the child is still running
            // If result is >0, the child has exited but wasn't reaped

            if (result > 0) {
                std::cerr << "Zombie process cleanup failed to reap child process" << std::endl;
                allTestsPassed = false;
            }

            // Check if the child process ran correctly
            std::ifstream file(flagFile);
            bool fileExists = file.good();
            file.close();

            // Clean up
            unlink(flagFile.c_str());

            if (!fileExists) {
                std::cerr << "Child process didn't execute correctly" << std::endl;
                allTestsPassed = false;
            }
        } else {
            // Fork failed
            std::cerr << "Failed to fork child process for zombie test" << std::endl;
            allTestsPassed = false;
        }
    }

    return allTestsPassed;
}
