# Contributing to ninxsh

Thank you for your interest in contributing to ninxsh! This document provides guidelines and instructions for contributing to this modern, feature-rich shell project.

## Table of Contents

- [Contributing to ninxsh](#contributing-to-ninxsh)
  - [Table of Contents](#table-of-contents)
  - [Code of Conduct](#code-of-conduct)
  - [Getting Started](#getting-started)
    - [Development Environment](#development-environment)
    - [Building the Project](#building-the-project)
      - [Universal Build (Cross-Platform)](#universal-build-cross-platform)
      - [Platform-Optimized Builds](#platform-optimized-builds)
      - [IDE Support and Development Tools](#ide-support-and-development-tools)
      - [Running and Testing](#running-and-testing)
  - [How to Contribute](#how-to-contribute)
    - [Reporting Bugs](#reporting-bugs)
    - [Suggesting Enhancements](#suggesting-enhancements)
    - [Pull Requests](#pull-requests)
  - [Coding Guidelines](#coding-guidelines)
    - [C++ Style Guide](#c-style-guide)
    - [Commit Messages](#commit-messages)
  - [Project Structure](#project-structure)
  - [Testing](#testing)
    - [Automated Test Suite](#automated-test-suite)
    - [Manual Testing](#manual-testing)
  - [Development Workflow](#development-workflow)
    - [Feature Development](#feature-development)
    - [Bug Fixing](#bug-fixing)
    - [Code Review Process](#code-review-process)
  - [Communication](#communication)

## Code of Conduct

This project and everyone participating in it is governed by our Code of Conduct. By participating, you are expected to uphold this code. Please report unacceptable behavior to the project maintainers.

## Getting Started

### Development Environment

To set up your development environment for ninxsh:

1. Clone the repository:

   ```bash
   git clone https://github.com/AarambhaAnta/ninxsh.git
   cd ninxsh
   ```

2. Install required dependencies:
   - **Compiler**: GCC 7+ or Clang 6+ with C++17 support
   - **Build System**: Make
   - **Platform Tools**: Platform-specific development tools (see below)
   - **Optional**: Bear for generating compile_commands.json
   - **Optional**: clang-format for code formatting
   - **Optional**: Debugging tools (Valgrind, GDB, lldb)

3. Platform-specific setup:
   - **macOS**: Xcode Command Line Tools (`xcode-select --install`)
   - **Linux**: Essential build tools (`sudo apt install build-essential` or equivalent)
   - **Windows**: MSYS2 or MinGW-w64 environment

### Building the Project

ninxsh provides multiple build options to suit different development needs and platforms.

#### Universal Build (Cross-Platform)

The main Makefile provides cross-platform compatibility:

```bash
make            # Standard build
make debug      # Debug build with symbols and debugging info
make release    # Optimized release build (-O3)
make sanitize   # Build with AddressSanitizer and UBSanitizer
make test       # Build and run comprehensive test suite (9 test categories)
make clean      # Clean all build artifacts
```

#### Platform-Optimized Builds

For platform-specific features, optimizations, and packaging:

```bash
# macOS (Xcode toolchain, framework linking, app bundles)
cd Resources/Mac && make
cd Resources/Mac && make test    # Platform-specific test optimizations

# Linux (distribution detection, shared libraries, packaging)
cd Resources/Linux && make
cd Resources/Linux && make deps  # Auto-install dependencies
cd Resources/Linux && make test

# Windows (MinGW/MSYS2, static linking, installers)
cd Resources/Windows && make
cd Resources/Windows && make test
```

#### IDE Support and Development Tools

For optimal development experience:

```bash
# Generate compile_commands.json for IDE support
make clean
bear -- make
# Or platform-specific: cd Resources/[Mac|Linux|Windows] && make compile_commands

# Code formatting (requires clang-format)
make format     # Format all source files
```

#### Running and Testing

```bash
# Basic usage
./bin/ninxsh              # Run the interactive shell

# Comprehensive testing
make test                 # Run all 9 test categories (100% pass rate)

# Individual test categories
./bin/test_runner builtin     # Test built-in commands (cd, exit, jobs, etc.)
./bin/test_runner command     # Test command parsing and tokenization
./bin/test_runner executor    # Test command execution and pipelines
./bin/test_runner jobs        # Test job control (bg, fg, kill)
./bin/test_runner utils       # Test utility functions and security
./bin/test_runner integration # Test complete shell workflows

# Development cleanup
make clean                # Remove build artifacts but keep tests
make distclean           # Complete cleanup including test binaries
```

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue on GitHub with the following information:

- A clear, descriptive title
- Steps to reproduce the issue
- Expected behavior
- Actual behavior
- Environment information (OS, compiler version, etc.)
- Any additional context or screenshots

### Suggesting Enhancements

Enhancement suggestions are welcome! Please create an issue with:

- A clear, descriptive title
- Detailed description of the proposed enhancement
- Specific examples of how the enhancement would work
- Why this enhancement would be useful to most ninxsh users

### Pull Requests

Before submitting a pull request:

1. **Fork and branch**: Fork the repository and create a feature branch from `main`

   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b fix/bug-description
   ```

2. **Develop and test**: Make your changes following our coding guidelines
3. **Test thoroughly**: Ensure all tests pass (`make test` should show 9/9 passed)
4. **Commit clearly**: Use descriptive commit messages (see [Commit Messages](#commit-messages))
5. **Update documentation**: Update relevant documentation if needed
6. **Push and PR**: Push your branch and open a pull request

**Pull Request Requirements:**

- **Description**: Clear description of changes and motivation
- **Testing**: Evidence that tests pass and new functionality is tested
- **Documentation**: Updated docs for user-facing changes
- **Compatibility**: Ensure changes work across supported platforms
- **Code Quality**: Follow established coding standards and patterns

**Pull Request Checklist:**

- [ ] All tests pass (`make test` shows 9/9 passed)
- [ ] New functionality includes appropriate tests
- [ ] Code follows the established style guide
- [ ] Documentation updated for user-facing changes
- [ ] Platform compatibility maintained (Mac/Linux/Windows)
- [ ] No regression in existing functionality
- [ ] Security considerations addressed (if applicable)

Pull request titles should be descriptive and follow the commit message format when possible.

## Coding Guidelines

### C++ Style Guide

We maintain a consistent coding style across the entire codebase:

**Naming Conventions:**

- Classes and structs: `PascalCase` (e.g., `JobManager`, `CommandParser`)
- Functions and methods: `camelCase` (e.g., `executeCommand`, `parseInput`)
- Variables: `camelCase` (e.g., `commandLine`, `jobId`)
- Constants and macros: `UPPER_SNAKE_CASE` (e.g., `MAX_COMMAND_LENGTH`)
- Member variables: `camelCase` (no `m_` prefix)
- File names: `snake_case` for source files (e.g., `job_manager.cpp`)

**Formatting:**

- **Indentation**: 4 spaces (no tabs)
- **Braces**: Opening braces on same line for functions and control structures
- **Spacing**: Space between control statements and parentheses (`if (condition)`)
- **Line Length**: Aim for 80-100 characters per line
- **Control Structures**: Always use braces, even for single statements

**Modern C++ Practices:**

- Use `nullptr` instead of `NULL` or `0`
- Prefer `auto` for complex type declarations when type is obvious
- Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) over raw pointers
- Prefer range-based for loops when appropriate
- Use `const` wherever possible
- Avoid `using namespace std` in header files
- Use `#pragma once` or include guards for all headers

**Code Organization:**

- Include order: Standard library, third-party, project headers
- Group related functionality together
- Keep functions focused and reasonably sized
- Use meaningful variable and function names
- Add comments for complex logic or algorithms

**Automated Code Formatting:**

We provide a `.clang-format` file for consistent formatting. Use it with:

```bash
# Format specific files
clang-format -i src/your_file.cpp

# Format all source files
make format

# Or manually for all files
find src include tests -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

Most IDEs support clang-format integration for automatic formatting.

### Commit Messages

Write clear commit messages that explain the "what" and "why" of your changes:

- Use the imperative mood ("Add feature" not "Added feature" or "Adds feature")
- First line should be a concise summary (<50 characters)
- Followed by a blank line
- Optionally followed by a more detailed explanation
- Reference issue numbers if applicable

We follow this commit message convention: `type: short summary`

| Type     | Use for...                                           | Examples                                    |
| -------- | ---------------------------------------------------- | ------------------------------------------- |
| feat     | Adding new features                                  | job control, command history, auto-complete |
| fix      | Fixing bugs                                          | memory leaks, parsing errors, crashes      |
| refactor | Code restructuring without behavior changes          | improving code organization, performance    |
| docs     | Documentation updates                                | README, comments, contributing guidelines   |
| test     | Adding or modifying tests                            | new test cases, test infrastructure         |
| style    | Code formatting and style changes only               | clang-format, indentation, naming           |
| perf     | Performance improvements                             | optimization, algorithm improvements        |
| security | Security-related changes                             | input validation, DoS protection           |
| chore    | Build process, dependencies, maintenance             | Makefile updates, dependency upgrades      |

Example:

```text
feat: Add comprehensive job control system

- Implement JobManager for background job tracking
- Add built-in commands: jobs, kill, fg, bg
- Integrate job tracking with signal handling
- Add comprehensive test suite for job control

Closes #42, addresses requirements from #38
```

## Project Structure

ninxsh follows a modular architecture that separates concerns and enables maintainable development:

```bash
ninxsh/
├── src/                    # Core implementation
│   ├── main.cpp           # Main entry point and shell loop
│   ├── shell.cpp          # Shell class with interactive logic
│   ├── command.cpp        # Command parsing and tokenization
│   ├── executor.cpp       # Command execution and pipelines
│   ├── builtin.cpp        # Built-in command implementations
│   ├── jobs.cpp          # Job control and process management
│   └── utils.cpp          # Utility functions and helpers
├── include/               # Header files
│   ├── shell.hpp          # Shell class and main logic
│   ├── command.hpp        # Command parsing structures
│   ├── executor.hpp       # Execution engine interface
│   ├── builtin.hpp        # Built-in command declarations
│   ├── jobs.hpp           # Job management system
│   ├── limits.hpp         # Security and resource limits
│   └── utils.hpp          # Utility function declarations
├── tests/                 # Comprehensive test suite
│   ├── test_runner.cpp    # Test framework and runner
│   ├── test_builtin.cpp   # Built-in command tests
│   ├── test_command.cpp   # Command parsing tests
│   ├── test_executor.cpp  # Execution engine tests
│   ├── test_jobs.cpp      # Job control tests
│   ├── test_utils.cpp     # Utility function tests
│   └── test_integration.cpp # End-to-end integration tests
├── Resources/             # Platform-specific builds
│   ├── Mac/               # macOS-optimized Makefile
│   ├── Linux/             # Linux-optimized Makefile
│   └── Windows/           # Windows-optimized Makefile
├── bin/                   # Built executables (created during build)
├── obj/                   # Object files (created during build)
├── Makefile              # Main cross-platform build system
├── .clang-format         # Code formatting configuration
├── .gitignore            # Git ignore patterns
├── compile_commands.json # IDE support (generated by bear)
├── README.md             # Main project documentation
├── CONTRIBUTING.md       # This file
└── LICENSE               # Project license
```

**Key Architecture Principles:**

- **Modularity**: Each component has a single responsibility
- **Separation**: Headers and implementation are clearly separated
- **Testability**: All components are designed to be easily testable
- **Portability**: Platform-specific code is isolated in Resources/
- **Maintainability**: Clear naming and consistent organization

**When Adding New Functionality:**

- Follow the established module pattern (implementation + header)
- Add appropriate tests in the `tests/` directory
- Update relevant Makefiles if adding new source files
- Consider platform compatibility across Mac/Linux/Windows
- Document new features in README.md
- Follow the security guidelines established in `include/limits.hpp`

## Testing

ninxsh features a comprehensive, automated test suite ensuring reliability and regression protection.

### Automated Test Suite

Our test framework covers all major components with 9 test categories:

```bash
# Run all tests (should show 9/9 passed)
make test

# Run individual test categories
./bin/test_runner builtin      # Built-in commands (cd, exit, jobs, kill, fg, bg)
./bin/test_runner command      # Command parsing and tokenization
./bin/test_runner executor     # Command execution and pipeline handling
./bin/test_runner jobs         # Job control and process management
./bin/test_runner utils        # Utility functions and security features
./bin/test_runner integration  # End-to-end shell workflows
./bin/test_runner signal       # Signal handling and cleanup
./bin/test_runner io_pipeline  # I/O redirection and pipelines
./bin/test_runner all          # Run all test categories
```

**Test Coverage:**

- **Built-in Commands**: All shell built-ins (cd, exit, jobs, kill, fg, bg)
- **Command Parsing**: Tokenization, quoting, special characters
- **Execution Engine**: External commands, pipelines, I/O redirection
- **Job Control**: Background jobs, process group management, signal handling
- **Security**: Input validation, DoS protection, resource limits
- **Integration**: Complete shell workflows and edge cases
- **Platform Compatibility**: Cross-platform behavior verification

**Test Guidelines:**

When contributing new features or fixes:

1. **Write Tests First**: Follow TDD when possible
2. **Cover Edge Cases**: Test boundary conditions and error cases
3. **Maintain Coverage**: New features should include comprehensive tests
4. **Platform Testing**: Ensure tests pass on all supported platforms
5. **Performance Testing**: Consider adding benchmarks for performance-critical code

### Manual Testing

While automated tests cover most scenarios, manual testing is still valuable for:

- **Interactive Features**: Prompt behavior, tab completion, command history
- **User Experience**: Testing actual shell usage patterns
- **Performance**: Real-world performance under different workloads
- **Integration**: Testing with external tools and environments

**Manual Test Checklist:**

- [ ] Interactive shell starts and responds correctly
- [ ] All built-in commands work as expected
- [ ] External command execution works properly
- [ ] Pipeline and redirection function correctly
- [ ] Job control operates smoothly (bg, fg, jobs, kill)
- [ ] Signal handling behaves correctly (Ctrl+C, Ctrl+Z)
- [ ] Error messages are clear and helpful
- [ ] Performance is acceptable for typical usage

## Development Workflow

### Feature Development

1. **Plan**: Understand requirements and design approach
2. **Branch**: Create feature branch from main
3. **Develop**: Implement following coding guidelines
4. **Test**: Write and run tests (aim for 100% pass rate)
5. **Document**: Update relevant documentation
6. **Review**: Self-review code for quality and consistency
7. **Submit**: Create pull request with clear description

### Bug Fixing

1. **Reproduce**: Verify the bug and understand its scope
2. **Test**: Write failing test case that demonstrates the bug
3. **Fix**: Implement minimal fix that resolves the issue
4. **Verify**: Ensure fix resolves issue without side effects
5. **Regression**: Run full test suite to prevent regressions
6. **Document**: Update any relevant documentation

### Code Review Process

- **Automated Checks**: All PRs must pass automated testing
- **Manual Review**: Maintainers review code for quality and design
- **Testing**: Reviewers verify that tests adequately cover changes
- **Documentation**: Ensure user-facing changes are documented
- **Platform Compatibility**: Verify changes work across supported platforms

## Communication

We value open, transparent communication to build a strong community around ninxsh.

**Preferred Communication Channels:**

- **GitHub Issues**: Bug reports, feature requests, technical discussions
- **Pull Requests**: Code review, implementation discussions
- **GitHub Discussions**: General questions, ideas, community topics

**Communication Guidelines:**

- **Be Respectful**: Treat all community members with respect and kindness
- **Be Clear**: Provide context and details to help others understand
- **Be Patient**: Maintainers and contributors volunteer their time
- **Search First**: Check existing issues and discussions before creating new ones
- **Stay On Topic**: Keep discussions focused and relevant to the project

**Getting Help:**

- **Technical Issues**: Create a GitHub issue with detailed reproduction steps
- **Feature Ideas**: Open a GitHub discussion to gather community feedback
- **Build Problems**: Check platform-specific documentation in Resources/
- **General Questions**: Use GitHub discussions for broader questions

**Contributing to Discussions:**

- Share your experience and knowledge with other users
- Help answer questions from new contributors
- Provide feedback on proposed features and changes
- Report issues and suggest improvements

Thank you for contributing to ninxsh and helping make it a powerful, reliable shell for everyone!
