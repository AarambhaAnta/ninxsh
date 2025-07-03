#include "shell.hpp"

#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

#include "builtin.hpp"
#include "command.hpp"
#include "executor.hpp"

Shell::Shell() {
    // Load history from file if available
    history.loadFromFile();
}

Shell::~Shell() {
    // Save history to file when shell exits
    history.saveToFile();
}

void Shell::run() {
    setupSignalHandlers();
    setGlobalJobManager(&jobManager);  // Set global job manager for signal handlers
    std::string input;

    while (true) {
        cleanupZombieProcesses();
        jobManager.cleanupJobs();  // Clean up finished background jobs
        printPrompt();
        std::getline(std::cin, input);

        // Handle EOF (Ctrl+D)
        if (std::cin.eof()) {
            std::cout << '\n';
            break;
        }

        // Skip empty input
        if (input.empty())
            continue;

        // Check for history expansion (!n or !!)
        std::string expandedInput = expandHistoryCommand(input);
        if (expandedInput.empty()) {
            // History expansion failed
            continue;
        }

        // If the input was changed by history expansion, echo the command
        if (expandedInput != input) {
            std::cout << expandedInput << std::endl;
            input = expandedInput;
        }

        ParsedCommand parsed = parseCommand(input);

        // Check for parsing errors
        if (parsed.hasError) {
            std::cout << "ninxsh: " << parsed.errorMessage << "\n";
            continue;
        }

        // Skip if no commands were parsed
        if (parsed.pipeline.empty() || parsed.pipeline[0].args.empty()) {
            continue;
        }

        // Add valid command to history (after validation)
        history.addCommand(input);

        // Get the first command to check if it's a builtin
        std::string cmd = parsed.pipeline[0].args[0];

        // Check for history command
        if (cmd == "history" && parsed.pipeline.size() == 1) {
            std::vector<std::string> histArgs;
            for (auto arg : parsed.pipeline[0].args) {
                if (arg) {
                    histArgs.push_back(std::string(arg));
                }
            }
            displayHistory(histArgs);
            continue;
        }

        // Check for jobs command
        if (cmd == "jobs" && parsed.pipeline.size() == 1) {
            jobManager.printJobs();
            continue;
        }

        // Check for kill command
        if (cmd == "kill" && parsed.pipeline.size() == 1) {
            if (parsed.pipeline[0].args.size() > 1 && parsed.pipeline[0].args[1]) {
                try {
                    pid_t pid = std::stoi(parsed.pipeline[0].args[1]);
                    if (kill(pid, SIGTERM) == 0) {
                        std::cout << "Process " << pid << " terminated\n";
                    } else {
                        std::perror("kill");
                    }
                } catch (const std::exception& e) {
                    std::cout << "kill: invalid PID '" << parsed.pipeline[0].args[1] << "'\n";
                }
            } else {
                std::cout << "Usage: kill <pid>\n";
            }
            continue;
        }

        // Check for fg command (foreground)
        if (cmd == "fg" && parsed.pipeline.size() == 1) {
            if (parsed.pipeline[0].args.size() > 1 && parsed.pipeline[0].args[1]) {
                try {
                    int jobId = std::stoi(parsed.pipeline[0].args[1]);
                    Job* job = jobManager.findJobById(jobId);
                    if (job) {
                        std::cout << job->command << std::endl;
                        if (job->isStopped) {
                            // Resume the job
                            kill(job->pid, SIGCONT);
                            job->isStopped = false;
                            job->isRunning = true;
                        }
                        // Wait for the job to complete
                        int status;
                        waitpid(job->pid, &status, 0);
                        jobManager.removeJob(job->pid);
                    } else {
                        std::cout << "fg: job " << jobId << " not found\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "fg: invalid job ID '" << parsed.pipeline[0].args[1] << "'\n";
                }
            } else {
                // No job ID specified, use most recent job
                const auto& jobs = jobManager.getJobs();
                if (!jobs.empty()) {
                    Job* job = const_cast<Job*>(&jobs.back());
                    std::cout << job->command << std::endl;
                    if (job->isStopped) {
                        kill(job->pid, SIGCONT);
                        job->isStopped = false;
                        job->isRunning = true;
                    }
                    int status;
                    waitpid(job->pid, &status, 0);
                    jobManager.removeJob(job->pid);
                } else {
                    std::cout << "fg: no current job\n";
                }
            }
            continue;
        }

        // Check for bg command (background)
        if (cmd == "bg" && parsed.pipeline.size() == 1) {
            if (parsed.pipeline[0].args.size() > 1 && parsed.pipeline[0].args[1]) {
                try {
                    int jobId = std::stoi(parsed.pipeline[0].args[1]);
                    Job* job = jobManager.findJobById(jobId);
                    if (job) {
                        if (job->isStopped) {
                            // Resume the job in background
                            kill(job->pid, SIGCONT);
                            job->isStopped = false;
                            job->isRunning = true;
                            std::cout << "[" << job->jobId << "] " << job->command << " &\n";
                        } else {
                            std::cout << "bg: job " << jobId << " already running\n";
                        }
                    } else {
                        std::cout << "bg: job " << jobId << " not found\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "bg: invalid job ID '" << parsed.pipeline[0].args[1] << "'\n";
                }
            } else {
                // No job ID specified, use most recent stopped job
                const auto& jobs = jobManager.getJobs();
                Job* stoppedJob = nullptr;
                for (auto& job : jobs) {
                    if (job.isStopped) {
                        stoppedJob = const_cast<Job*>(&job);
                        break;
                    }
                }
                if (stoppedJob) {
                    kill(stoppedJob->pid, SIGCONT);
                    stoppedJob->isStopped = false;
                    stoppedJob->isRunning = true;
                    std::cout << "[" << stoppedJob->jobId << "] " << stoppedJob->command << " &\n";
                } else {
                    std::cout << "bg: no stopped jobs\n";
                }
            }
            continue;
        }

        // Only run builtins if it's a single command (not a pipeline)
        if (parsed.pipeline.size() == 1 && isBuiltin(cmd)) {
            executeBuiltin(parsed.pipeline[0].args);
            continue;
        }

        executeExternal(parsed, &jobManager);
    }
}

void Shell::printPrompt() const {
    std::cout << "🔮ninxsh > " << std::flush;
}

std::string Shell::expandHistoryCommand(const std::string& input) const {
    // If input is empty, no expansion needed
    if (input.empty()) {
        return input;
    }

    // Get all commands from history
    const auto& commands = history.getCommands();

    // Check for history expansions
    if (input == "!!") {
        // Execute the last command
        if (commands.empty()) {
            std::cout << "ninxsh: !!: event not found\n";
            return "";
        }
        return commands.back();
    } else if (input[0] == '!') {
        // Check for !n format (execute command number n)
        std::string numberStr = input.substr(1);
        try {
            int cmdNumber = std::stoi(numberStr);
            // History is 1-indexed in display but 0-indexed in storage
            size_t index = cmdNumber - 1;

            if (index >= commands.size()) {
                std::cout << "ninxsh: " << input << ": event not found\n";
                return "";
            }

            return commands[index];
        } catch (const std::exception& e) {
            // If not a valid number, treat as a regular command
            return input;
        }
    }

    // No expansion needed
    return input;
}

void Shell::displayHistory(const std::vector<std::string>& args) const {
    // Get the commands from history
    const auto& commands = history.getCommands();

    // Determine how many commands to display
    size_t numToDisplay = commands.size();

    // If an argument is provided, use it to limit the number of commands displayed
    if (args.size() > 1) {
        try {
            numToDisplay = std::min(numToDisplay, static_cast<size_t>(std::stoi(args[1])));
        } catch (const std::exception& e) {
            // If argument isn't a valid number, show all
        }
    }

    // Calculate the starting index
    size_t startIdx = (numToDisplay < commands.size()) ? commands.size() - numToDisplay : 0;

    // Display the commands with their indices
    for (size_t i = startIdx; i < commands.size(); ++i) {
        std::cout << (i + 1) << "  " << commands[i] << '\n';
    }
}
