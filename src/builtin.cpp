#include <unistd.h>
#include <iostream>
#include <vector>
#include <cstdlib>

bool executeBuiltin(const std::vector<char *> &argv)
{
    if (argv.empty() || argv[0] == nullptr)
        return false;
    std::string cmd = argv[0];

    if (cmd == "exit")
    {
        std::exit(0);
    }
    else if (cmd == "cd")
    {
        const char *path = (argv.size() > 1 && argv[1]) ? argv[1] : getenv("HOME");
        if (chdir(path) != 0)
        {
            std::perror("cd");
        }
    }
    else if (cmd == "clear")
    {
        std::cout << "\033[2J\033[0H" << std::flush;
    }
    else
    {
        return false;
    }
    return true;
}