#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <vector>

bool executeBuiltin(const std::vector<char*>& argv) {
    if (argv.empty() || argv[0] == nullptr)
        return false;
    std::string cmd = argv[0];

    if (cmd == "exit") {
        std::exit(0);
    } else if (cmd == "cd") {
        const char* path = (argv.size() > 1 && argv[1]) ? argv[1] : getenv("HOME");
        if (chdir(path) != 0) {
            std::perror("cd");
        }
    } else if (cmd == "clear") {
        std::cout << "\033[2J\033[0H" << std::flush;
    } else {
        return false;
    }
    return true;
}

bool executeBuiltin(const std::vector<const char*>& argv) {
    // Convert from const char* to char* for compatibility
    std::vector<char*> nonConstArgv;
    for (const char* arg : argv) {
        if (arg) {
            // This is safe since executeBuiltin doesn't modify the args
            nonConstArgv.push_back(const_cast<char*>(arg));
        } else {
            nonConstArgv.push_back(nullptr);
        }
    }
    return executeBuiltin(nonConstArgv);
}

bool isBuiltin(const std::string& cmd) {
    return cmd == "exit" || cmd == "clear" || cmd == "cd";
}
