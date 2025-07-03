#include "executor.hpp"

#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "command.hpp"
#include "jobs.hpp"

bool isShellForeground = true;
static JobManager* globalJobManager = nullptr;

void sigchldHandler(int /* sig */) {
    // Loop through all zombies and reap them
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Update job status if we have a job manager
        if (globalJobManager) {
            Job* job = globalJobManager->findJobByPid(pid);
            if (job) {
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    // Job finished, remove it
                    globalJobManager->removeJob(pid);
                }
            }
        }
    }
}

void sigintHandler(int /* sig */) {
    if (isShellForeground) {
        // If we're in the shell, just print a new prompt
        std::cout << "\n🔮ninxsh > ";
        std::cout.flush();
    }
    // If we're not in the shell, the default behavior will kill the foreground process
}

void sigtstpHandler(int /* sig */) {
    if (isShellForeground) {
        std::cout << "\n🔮ninxsh > ";
        std::cout.flush();
    }
}

void setupSignalHandlers() {
    struct sigaction sa;
    sa.sa_handler = sigchldHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        std::cout << "ninxsh: sigaction error for SIGCHLD\n";
    }

    // Set up SIGINT (Ctrl+C) handler
    sa.sa_handler = sigintHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        std::cout << "ninxsh: sigaction error for SIGINT\n";
    }

    // Set up SIGTSTP (Ctrl+Z) handler
    sa.sa_handler = sigtstpHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        std::cout << "ninxsh: sigaction error for SIGTSTP\n";
    }
}

void cleanupZombieProcesses() {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    }
}

void executeExternal(const ParsedCommand& cmd, JobManager* jobManager) {
    // If there's more than one command in the pipeline, use the pipeline executor
    if (cmd.pipeline.size() > 1) {
        executePipeline(cmd, jobManager);
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
        if (command.isBackground) {
            // Add job to job manager if provided
            if (jobManager) {
                std::string jobCommand = command.args[0];
                for (size_t i = 1; i < command.args.size() - 1;
                     ++i) {  // -1 because last element is nullptr
                    jobCommand += " " + std::string(command.args[i]);
                }
                int jobId = jobManager->addJob(pid, jobCommand);
                std::cout << "[" << jobId << "] " << pid << std::endl;
            } else {
                std::cout << "[1] " << pid << "\n";
            }
            isShellForeground = true;
        } else {
            isShellForeground = false;
            int status;
            waitpid(pid, &status, 0);
            isShellForeground = true;
        }
        cleanupZombieProcesses();
    }
}

void executePipeline(const ParsedCommand& cmd, JobManager* jobManager) {
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

    bool isBackground = cmd.pipeline[numCommands - 1].isBackground;

    if (isBackground) {
        // Add pipeline job to job manager if provided
        if (jobManager) {
            // Build command string for the entire pipeline
            std::string pipelineCommand;
            for (int i = 0; i < numCommands; ++i) {
                if (i > 0)
                    pipelineCommand += " | ";

                pipelineCommand += cmd.pipeline[i].args[0];
                for (size_t j = 1; j < cmd.pipeline[i].args.size() - 1;
                     ++j) {  // -1 because last element is nullptr
                    pipelineCommand += " " + std::string(cmd.pipeline[i].args[j]);
                }
            }
            int jobId = jobManager->addJob(pids[numCommands - 1], pipelineCommand);
            std::cout << "[" << jobId << "] " << pids[numCommands - 1] << std::endl;
        } else {
            std::cout << "[1] " << pids[numCommands - 1] << "\n";
        }
        isShellForeground = true;
    } else {
        isShellForeground = false;
        // Wait for all the child processes to complete
        for (int i = 0; i < numCommands; i++) {
            int status;
            waitpid(pids[i], &status, 0);
        }
        isShellForeground = true;
    }

    cleanupZombieProcesses();
}

void setGlobalJobManager(JobManager* jobManager) {
    globalJobManager = jobManager;
}
