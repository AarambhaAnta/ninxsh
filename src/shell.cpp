#include "shell.hpp"
#include "command.hpp"
#include "builtin.hpp"

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

void Shell::run()
{
    std::string input;
    while (true)
    {
        printPrompt();
        std::getline(std::cin, input);

        // Handle EOF (Ctrl + C)
        if (std::cin.eof())
        {
            std::cout << '\n';
            break;
        }

        // Skip empty input
        if (input.empty())
            continue;

        // Tokenize input into argv
        std::vector<char *> argv = tokenize(input);
        if(argv.empty()){
            continue;
        }

        std::string cmd = argv[0];

        // Check for builtin
        if(isBuiltin(cmd)){
            executeBuiltin(argv);
            // Free all arguments
            for(char *arg : argv){
                free(arg);
            }
            continue; // Skip external command execution for builtins
        }

        // External Commands
        pid_t pid = fork();
        if (pid < 0)
        {
            std::cerr << "ninxsh: fork failed\n";
            continue;
        }

        // command run
        if(pid==0){
            if(execvp(argv[0],argv.data())==-1){
                std::cerr << "ninxsh: command not found: " << argv[0] << "\n";
                exit(EXIT_FAILURE);
            }
        }else{
            int status;
            waitpid(pid, &status, 0);
        }
        // Free all arguments after external command execution
        for(char* arg : argv){
            free(arg);
        }

    }
}

void Shell::printPrompt() const
{
    std::cout << "🔮ninxsh > " << std::flush;
}