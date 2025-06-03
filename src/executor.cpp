#include "executor.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>

void executeExternal(const ParsedCommand &cmd)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        std::cerr << "ninxsh: fork failed\n";
        return;
    }

    if (pid == 0)
    {
        if (!cmd.inputFile.empty())
        {
            int fd = open(cmd.inputFile.c_str(), O_RDONLY);
            if (fd < 0)
            {
                std::perror("ninxsh: input redirection");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (!cmd.outputFile.empty())
        {
            int fd = open(cmd.outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                std::perror("ninxsh: output redirection");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd.args[0], cmd.args.data());
        std::cerr << "ninxsh: command not found: " << cmd.args[0] << "\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}