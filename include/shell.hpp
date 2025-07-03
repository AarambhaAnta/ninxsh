#ifndef SHELL_HPP
#define SHELL_HPP

#include "history.hpp"
#include "jobs.hpp"

class Shell {
private:
    History history;
    JobManager jobManager;
    void printPrompt() const;
    void displayHistory(const std::vector<std::string>& args) const;
    std::string expandHistoryCommand(const std::string& input) const;

public:
    Shell();
    ~Shell();
    void run();

    // Allow access to job manager for executor
    JobManager& getJobManager() {
        return jobManager;
    }
};

#endif  // SHELL_HPP
