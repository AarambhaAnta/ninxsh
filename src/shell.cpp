#include "shell.hpp"
#include "command.hpp"
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

        // Exit structures
        if (std::cin.eof())
        { /* Ctrl+C */
            std::cout << '\n';
            break;
        }
        if (input == "exit")
        {
            std::cout << '\n';
            break;
        }

        if (input.empty())
            continue;

        // child creation
        pid_t pid = fork();
        if(pid<0){
            std::cerr << "ninxsh: fork failed\n";
            continue;
        }

        // argument tokenization
        std::vector<char *> argv = tokenize(input);
        // command run
        if(pid==0){
            if(execvp(argv[0],argv.data())==-1){
                std::cerr << "ninxsh: command not found: " << argv[0] << "\n";
                exit(EXIT_FAILURE);
            }
        }else{
            int status;
            waitpid(pid, &status, 0);
            for(char* arg : argv){
                free(arg);
            }
        }

    }
}

void Shell::printPrompt() const
{
    std::cout << "🔮ninxsh > " << std::flush;
}