#ifndef SHELL_HPP
#define SHELL_HPP

#include<string>

class Shell{
    private:
        void printPrompt() const;
    public:
        void run();
};

#endif  // SHELL_HPP