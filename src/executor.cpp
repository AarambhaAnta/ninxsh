#include "executor.hpp"

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "command.hpp"

void executeExternal(const ParsedCommand& cmd) {
    // If there's more than one command in the pipeline, use the pipeline executor
    if (cmd.pipeline.size() > 1) {
        executePipeline(cmd);
        return;
    }

    // Otherwise execute a single command (the first/only one in the pipeline)
    const Command& command = cmd.pipeline[0];
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "ninxsh: fork failed\n";
        return;
    }

    if (pid == 0) {
        if (!command.inputFile.empty()) {
            int fd = open(command.inputFile.c_str(), O_RDONLY);
            if (fd < 0) {
                std::perror("ninxsh: input redirection");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (!command.outputFile.empty()) {
            int fd = open(command.outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                std::perror("ninxsh: output redirection");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(command.args[0], command.args.data());
        std::cerr << "ninxsh: command not found: " << command.args[0] << "\n";
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

void executePipeline(const ParsedCommand& cmd) {
    int numCommands = cmd.pipeline.size();
    std::vector<int> pipeFds((numCommands - 1) * 2);  // Each pipe has 2 file descriptors

    // Create all the pipes needed
    for (int i = 0; i < numCommands - 1; i++) {
        if (pipe(&pipeFds[i * 2]) < 0) {
            std::cerr << "ninxsh: failed to create pipe\n";
            return;
        }
    }

    std::vector<pid_t> pids(numCommands);

    // Fork and execute each command in the pipeline
    for (int i = 0; i < numCommands; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            std::cerr << "ninxsh: fork failed\n";
            return;
        }

        if (pids[i] == 0) {
            // Child process
            //
            // Setup input
            if (i == 0) {
                // First command: handle input redirection
                if (!cmd.pipeline[i].inputFile.empty()) {
                    int fd = open(cmd.pipeline[i].inputFile.c_str(), O_RDONLY);
                    if (fd < 0) {
                        std::perror("ninxsh: input redirection");
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }
            } else {
                // Not first command: read from previous pipe
                dup2(pipeFds[(i - 1) * 2], STDIN_FILENO);
            }

            // Setup output
            if (i == numCommands - 1) {
                // Last command: handle output redirection
                if (!cmd.pipeline[i].outputFile.empty()) {
                    int fd = open(cmd.pipeline[i].outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC,
                                  0644);
                    if (fd < 0) {
                        std::perror("ninxsh: output redirection");
                        exit(EXIT_FAILURE);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            } else {
                // Not last command: write to next pipe
                dup2(pipeFds[i * 2 + 1], STDOUT_FILENO);
            }
            // Close all pipe file descriptors
            for (int j = 0; j < (numCommands - 1) * 2; j++) {
                close(pipeFds[j]);
            }

            // Execute the command
            execvp(cmd.pipeline[i].args[0], cmd.pipeline[i].args.data());
            std::cerr << "ninxsh: command not found: " << cmd.pipeline[0].args[0] << "\n";
            exit(EXIT_FAILURE);
        }
    }

    // Parent process
    //
    // Close all pipe file descriptors in the present
    for (int i = 0; i < (numCommands - 1) * 2; i++) {
        close(pipeFds[i]);
    }

    // Wait for all the child processes to complete
    for (int i = 0; i < numCommands; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }
}
