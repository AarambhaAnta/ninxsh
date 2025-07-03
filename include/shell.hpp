#ifndef SHELL_HPP
#define SHELL_HPP

#include "history.hpp"

class Shell {
private:
    History history;
    void printPrompt() const;
    void displayHistory(const std::vector<std::string>& args) const;
    std::string expandHistoryCommand(const std::string& input) const;

public:
    Shell();
    ~Shell();
    void run();
};

#endif  // SHELL_HPP
