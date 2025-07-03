# ninxsh - A Custom Unix Shell

![LICENSE](https://img.shields.io/badge/license-MIT-green.svg)
![Last Commit](https://img.shields.io/github/last-commit/AarambhaAnta/ninxsh)
![Language](https://img.shields.io/github/languages/top/AarambhaAnta/ninxsh)
![Stars](https://img.shields.io/github/stars/AarambhaAnta/ninxsh?style=social)

`ninxsh` is a lightweight shell written in modern C++. It supports:

- **Enhanced Terminal Prompt**: Modern `username@hostname:path$` format with ANSI colors
- **Smart Prompt Display**: Colored prompt for interactive use, plain format when piped
- **Builtin Commands** (`exit`, `cd`, `clear`, `history`, `jobs`, `kill`, `fg`, `bg`)
- **External executable support** using `fork()` and `execvp()`
- **Input/output redirection** (`<`, `>`)
- **Command pipelines** (`|`) with multiple commands
- **Background process execution** (`&`)
- **Job control and management** (`jobs`, `kill <pid>`, `fg [job_id]`, `bg [job_id]`)
- **Signal handling** (Ctrl+C, Ctrl+Z)
- **Path expansion** (`~` to home directory)
- **Environment variable expansion** (`$HOME`, `$USER`, etc.)
- **Advanced quote handling** (single quotes, double quotes, escape sequences)
- **Zombie process cleanup** with automatic job status updates
- DoS protection with configurable limits (centralized in `limits.hpp`)
- Comprehensive test suite for all features
- Command history with persistent storage and execution (`!!`, `!n`)

## Build Instructions

### Universal Build (Cross-Platform)

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

### Platform-Optimized Builds

For platform-specific optimizations and features, use the specialized Makefiles:

```bash
# macOS (Xcode toolchain, code signing, app bundles)
cd Resources/Mac && make

# Linux (distribution detection, package creation, valgrind)
cd Resources/Linux && make

# Windows (MinGW/MSYS2, installers, portable builds)
cd Resources/Windows && make
```

#### Platform-Specific Features

**macOS** (`Resources/Mac/`):

- Debug symbols for lldb (`make dsym`)
- Code signing (`make codesign`)
- App bundle creation (`make bundle`)
- Apple Silicon support (`make install-arm64`)

**Linux** (`Resources/Linux/`):

- Auto-detect distribution and package manager
- Memory testing (`make valgrind`)
- Performance profiling (`make perf`)
- Package creation (`make deb`, `make rpm`)
- Static builds (`make static`)

**Windows** (`Resources/Windows/`):

- NSIS installer creation (`make installer`)
- Portable ZIP packages (`make portable`)
- Static linking for portability
- MSYS2 dependency management (`make deps`)

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
│   ├── history.cpp     # Command history
│   └── jobs.cpp        # Job management
├── include/
│   ├── shell.hpp
│   ├── command.hpp
│   ├── executor.hpp
│   ├── builtin.hpp
│   ├── utils.hpp
│   ├── history.hpp
│   ├── jobs.hpp
│   └── limits.hpp      # DoS protection constants
├── tests/
│   ├── test_runner.cpp         # Test runner framework
│   ├── test_command.cpp        # Command parsing tests
│   ├── test_builtin.cpp        # Builtin command tests
│   ├── test_utils.cpp          # Utility function tests
│   ├── test_executor.cpp       # Command execution tests
│   ├── test_io_pipeline.cpp    # I/O and pipeline tests
│   ├── test_signal.cpp         # Signal handling tests
│   ├── test_history.cpp        # Command history tests
│   ├── test_dos_protection.cpp # DoS protection tests
│   └── test_jobs.cpp           # Job management tests
├── Resources/
│   ├── Mac/Makefile            # macOS-optimized build
│   ├── Linux/Makefile          # Linux-optimized build
│   └── Windows/Makefile        # Windows-optimized build
├── Makefile                    # Universal build system
├── README.md                   # Project documentation
└── .gitignore                  # Git ignore file
```

> To generate the initial project structure:

```bash
mkdir -p ninxsh/{src,include,tests,Resources/{Mac,Linux,Windows}} \
&& touch ninxsh/src/{main.cpp,shell.cpp,command.cpp,executor.cpp,builtin.cpp,utils.cpp,history.cpp,jobs.cpp} \
&& touch ninxsh/include/{shell.hpp,command.hpp,executor.hpp,builtin.hpp,utils.hpp,history.hpp,jobs.hpp,limits.hpp} \
&& touch ninxsh/tests/{test_runner.cpp,test_command.cpp,test_builtin.cpp,test_utils.cpp,test_executor.cpp,test_io_pipeline.cpp,test_signal.cpp,test_history.cpp,test_dos_protection.cpp,test_jobs.cpp} \
&& touch ninxsh/Resources/{Mac,Linux,Windows}/Makefile \
&& touch ninxsh/{Makefile,README.md,.gitignore}
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
- [x] Implemented `history` with persistent storage and execution (`!!`, `!n`)
- [x] Implemented `jobs` for listing background jobs
- [x] Implemented `kill <pid>` for terminating processes
- [x] Implemented `fg [job_id]` for bringing jobs to foreground
- [x] Implemented `bg [job_id]` for resuming jobs in background
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

### **Day 10** - Security & Performance Improvements

- [x] Added explicit error state handling to ParsedCommand structure
- [x] Implemented comprehensive DoS protection with input length validation
- [x] Prevented history pollution by validating commands before storage
- [x] Centralized all protection limits in `include/limits.hpp` header
- [x] Optimized regex compilation with static const patterns
- [x] Standardized test file naming to `test_*.cpp` convention
- [x] Added boundary condition tests and enhanced error reporting
- [x] Updated documentation with security features and architecture

### **Day 11** - Terminal Improvements

- [x] Enhanced prompt format: Modern `username@hostname:path$` replacing emoji prompt
- [x] ANSI colored prompt for interactive terminal sessions
  - [x] Username in bold green, hostname in blue, path in yellow
  - [x] Smart home directory replacement with `~`
  - [x] Intelligent path truncation for very long directories
- [x] Smart terminal detection with `isOutputToTerminal()`
- [x] Plain text prompt when output is piped to prevent formatting issues
- [x] Cross-platform compatibility with robust fallbacks
- [x] Added complete ANSI Colors namespace (16 colors + formatting)
- [x] Comprehensive terminal utility functions (`getCurrentUsername`, `getCurrentHostname`, `getCurrentWorkingDir`)
- [x] Enhanced test coverage for all terminal functionality
- [x] Updated documentation with new terminal features

### **Day 12** - Advanced Quote Handling

- [x] Comprehensive quote parsing with new tokenizer in `src/command.cpp`
- [x] Proper single quote handling (literal strings, no variable expansion)
- [x] Proper double quote handling (allows variable expansion)
- [x] Escape sequence support with backslash (`\`) for special characters
- [x] Mixed quoting support (single, double, and unquoted in same command)
- [x] Quote-aware environment variable expansion (respects quote context)
- [x] Pipeline support with quoted arguments
- [x] Comprehensive test suite in `tests/test_quotes.cpp` with 7 test cases
- [x] Fixed `expandPath()` to only handle tilde (`~`) expansion, not environment variables
- [x] All tests passing (10/10) including quote handling integration

## Security Features

### DoS Protection

- **Input Length Validation**: Commands longer than 4KB are rejected
- **Path Length Limits**: File paths longer than 2KB are handled gracefully  
- **String Processing Limits**: Environment variable expansion limited to 2KB strings
- **Centralized Configuration**: All limits defined in `include/limits.hpp`
- **Explicit Error Handling**: Clear error messages for rejected input
- **History Protection**: Invalid commands are not stored in command history

### Performance Optimizations

- **Static Regex Compilation**: Environment variable patterns compiled once
- **Early Exit Strategies**: Skip expensive operations when possible
- **Memory Management**: Proper cleanup with RAII patterns

## Testing

The ninxsh project includes a comprehensive C++ test suite covering all major functionality.

### Running All Tests

```bash
make test    # Run complete test suite
```

The test suite includes:

- **Command parsing** - Input tokenization and validation
- **Builtin commands** - All builtin functions (cd, exit, clear, history, jobs, kill, fg, bg)
- **External execution** - Fork/exec functionality and process management
- **I/O redirection & pipelines** - File redirection and command chaining
- **Signal handling** - Ctrl+C, Ctrl+Z, and process cleanup
- **Job management** - Background job tracking and control
- **History functionality** - Command storage and expansion
- **DoS protection** - Input validation and security limits
- **Utility functions** - Helper and utility code

All tests use a unified framework and report PASSED/FAILED status.

### Manual Testing Examples

```bash
# Test normal operation
echo "hello world"
cd ~
ls | grep txt

# Test history
history
!!
!1

# Test environment variables
echo $HOME
cd $HOME

# Test background jobs and job control
sleep 30 &           # Start background job
jobs                 # List active jobs
kill 1234            # Kill process by PID
fg 1                 # Bring job 1 to foreground
bg 1                 # Resume job 1 in background

# Test pipelines with background
ls -la | grep txt &  # Run pipeline in background
jobs                 # View the background pipeline
```

---

## Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute to this project.

### Development Workflow

1. **Fork** the repository
2. **Clone** your fork locally
3. **Create** a feature branch (`git checkout -b feature/amazing-feature`)
4. **Make** your changes with proper tests
5. **Test** thoroughly (`make test`)
6. **Format** code (`make format`)
7. **Commit** with descriptive messages
8. **Push** to your branch (`git push origin feature/amazing-feature`)
9. **Open** a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributors

Special thanks to the following people who have contributed to making ninxsh better:

- **[@piotrmaciejbednarski](https://github.com/piotrmaciejbednarski)** - Discovered and reported critical DoS vulnerability in input processing, provided detailed reproduction steps and suggested security improvements that led to the implementation of comprehensive DoS protection in Day 10

*Want to see your name here? Check out our [Contributing Guidelines](CONTRIBUTING.md) and submit a pull request!*

## Acknowledgments

- Built with modern C++17 standards
- Inspired by traditional Unix shells (bash, zsh, tcsh)
- Cross-platform compatibility with macOS, Linux, and Windows
- Comprehensive testing and documentation practices

## Contact & Support

- **Issues**: Report bugs and request features via [GitHub Issues](https://github.com/AarambhaAnta/ninxsh/issues)
- **Discussions**: Join conversations in [GitHub Discussions](https://github.com/AarambhaAnta/ninxsh/discussions)
- **Documentation**: Full documentation available in this README and [CONTRIBUTING.md](CONTRIBUTING.md)

---

<div align="center">

**ninxsh** - A modern Unix shell written in C++

⭐ **Star this repo** if you find it useful! ⭐

Built with ❤️ by developers, for developers

</div>
