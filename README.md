# ninxsh - A Custom Unix Shell

![LICENSE](https://img.shields.io/badge/license-MIT-green.svg)
![Last Commit](https://img.shields.io/github/last-commit/AarambhaAnta/ninxsh)
![Language](https://img.shields.io/github/languages/top/AarambhaAnta/ninxsh)
![Stars](https://img.shields.io/github/stars/AarambhaAnta/ninxsh?style=social)

`ninxsh` is a lightweight shell written in modern C++. It supports:

- REPL prompt with custom emoji 🔮
- Builtin Commands (`exit`, `cd`, `clear`, `history`)
- External executable support using `fork()` and `execvp()`
- Input/output redirection (`<`, `>`)
- Command pipelines (`|`) with multiple commands
- Background process execution (`&`)
- Signal handling (Ctrl+C, Ctrl+Z)
- Path expansion (`~` to home directory)
- Environment variable expansion (`$HOME`, `$USER`, etc.)
- Zombie process cleanup
- Comprehensive test suite for all features
- Command history with persistent storage and execution (`!!`, `!n`)
- Quoting (planned)

## Build Instructions

```bash
make         # Build the shell in default mode
make debug   # Build with debug symbols and without optimizations
make release # Build with optimizations
make sanitize # Build with sanitizers for catching memory issues
make format  # Format code using clang-format
make test    # Run the test suite
make install # Install the shell (requires sudo)
make clean   # Delete build artifacts
make help    # Show available commands
```

## Developer Setup

To enable IDE features like auto-complete and jump-to-definition, generate a `compile_commands.json`:

```bash
make clean
bear -- make
```

## Project Structure

```bash
ninxsh/
├── src/
│   ├── main.cpp        # Main loop
│   ├── shell.cpp       # Shell class logic
│   ├── command.cpp     # Tokenization
│   ├── executor.cpp    # External execution logic
│   ├── builtin.cpp     # Built-in command handlers
│   ├── utils.cpp       # Misc utilities
│   └── history.cpp     # (Optional) Command history
├── include/
│   ├── shell.hpp
│   ├── command.hpp
│   ├── executor.hpp
│   ├── builtins.hpp
│   ├── util.hpp
│   └── history.hpp
├── tests/
│   ├── test_runner.cpp         # Test runner framework
│   ├── command_test.cpp        # Command parsing tests
│   ├── builtin_test.cpp        # Builtin command tests
│   ├── utils_test.cpp          # Utility function tests
│   ├── executor_test.cpp       # Command execution tests
│   ├── io_pipeline_test.cpp    # I/O and pipeline tests
│   ├── signal_test.cpp         # Signal handling tests
│   └── README.md               # Test documentation
├── Makefile                    # Build system
├── README.md                   # Project documentation
└── .gitignore                  # Git ignore file
```

> To generate the initial project structure:

```bash
mkdir -p ninxsh/src ninxsh/include \
&& touch ninxsh/src/{main.cpp,shell.cpp,command.cpp,executor.cpp,builtin.cpp,utils.cpp,history.cpp} \
&& touch ninxsh/include/{shell.hpp,command.hpp,executor.hpp,builtins.hpp,util.hpp,history.hpp} \
&& touch ninxsh/Makefile ninxsh/README.md ninxsh/.gitignore
```

---

## Progress Roadmap

### **Day 1** - Shell Initialization

- [x] Project compiles with `make`
- [x] **Print Prompt:** `ninxsh >`
- [x] Reads user input
- [x] Gracefully handles empty input and `Ctrl + C` (EOF)
- [x] Initial commits in Git

### **Day 2** - External Command Execution

- [x] Run external commands using `fork()` and `execvp()`
- [x] Supports basic Unix commands like `ls`, `pwd`, `echo`, `date`, etc.
- [x] Basic error handling for invalid commands (shows "command not found")
- [x] Robust input tokenization and conversion to C-style argument arrays
- [x] Handles command execution filure gracefully without crashing
- [x] Proper memory management with `strdup()` and `free()`

### **Day 3** - Builtin Commands

- [x] Implemented `exit`
- [x] Implemented `cd [path]` (fallback `$HOME`)
- [x] Implemented `clear` using ANSI escape sequence
- [x] `Builtins` are handled in parent process (no `fork`)

### **Day 4** - Path resolution

- [x] Handle absolute & relative path commands
- [x] Support for `~` expansion to `$HOME`
- [x] Support for environment variable expansion like `$HOME`, `$USER`

### **Day 5** - I/O Redirection

- [x] Input Redirection (`command < file.txt`)
- [x] Output Redirection (`command > file.txt`)
- [x] Used `dup2()` for redirection
- [x] Refactored execution into `executor.cpp`

### **Day 6** - Command Pipelines

- [x] Support for piping commands (`ls -l | grep txt`)
- [x] Implemented multi-command pipeline execution with `pipe()` and multiple `fork()`s
- [x] Refactored `Command` and `ParsedCommand` to support pipelines
- [x] Added separate `executePipeline()` function for handling piped commands

### **Day 7** - Background Execution

- [x] Support for background processes with `&` (`sleep 10 &`)
- [x] Print job PID when running in background
- [x] Implemented SIGCHLD handler to avoid zombie processes
- [x] Used non-blocking `waitpid()` with `WNOHANG` for cleanup
- [x] Added proper signal handling with `sigaction()`

### **Day 8** - Robust Signal Handling & Testing

- [x] Implemented comprehensive signal handling (SIGINT, SIGTSTP, SIGCHLD)
- [x] Shell doesn't terminate on Ctrl+C or Ctrl+Z
- [x] Ensured proper foreground/background state management
- [x] Added zombie process cleanup system
- [x] Created comprehensive test suite with test runner
- [x] Added unit tests for commands, builtins, utils, and executor
- [x] Implemented integration tests for I/O redirection and pipelines
- [x] Added signal handling tests
- [x] Enhanced Makefile with debug/release/sanitize/format targets
- [x] Added test infrastructure with proper pattern rules

### **Day 9** - Command History

- [x] Implemented `history` command to view command history
- [x] Store previous commands in memory
- [x] Save history to `~/.ninxsh_history` file
- [x] Load history from file when shell starts
- [x] Automatic cleanup of oldest commands when history size exceeds limit
- [x] Support for `!!` to execute the last command
- [x] Support for `!n` to execute command number n from history
