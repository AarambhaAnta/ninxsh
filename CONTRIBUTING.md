# Contributing to ninxsh

Thank you for your interest in contributing to ninxsh! This document provides guidelines and instructions for contributing to this project.

## Table of Contents

- [Contributing to ninxsh](#contributing-to-ninxsh)
  - [Table of Contents](#table-of-contents)
  - [Code of Conduct](#code-of-conduct)
  - [Getting Started](#getting-started)
    - [Development Environment](#development-environment)
    - [Building the Project](#building-the-project)
  - [How to Contribute](#how-to-contribute)
    - [Reporting Bugs](#reporting-bugs)
    - [Suggesting Enhancements](#suggesting-enhancements)
    - [Pull Requests](#pull-requests)
  - [Coding Guidelines](#coding-guidelines)
    - [C++ Style Guide](#c-style-guide)
    - [Commit Messages](#commit-messages)
  - [Project Structure](#project-structure)
  - [Testing](#testing)
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
   - GCC or Clang compiler with C++14 support
   - Make build system
   - Optional: Bear for generating compile_commands.json

### Building the Project

Build the project with:

```bash
make
```

For IDE support (code completion, navigation), generate a compile_commands.json file:

```bash
make clean
bear -- make
```

Run the shell:

```bash
./ninxsh
```

Clean build artifacts:

```bash
make clean
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

1. Fork the repository
2. Create a new branch from `main` for your feature or bugfix

   ```bashbash
   git checkout -b feature/your-feature-name
   ```

3. Make your changes
4. Commit your changes with clear commit messages (see [Commit Messages](#commit-messages))
5. Push your branch to your fork
6. Open a pull request to the `main` branch of the original repository
7. Reference any related issues in your PR description

Pull request titles should be clear and descriptive. PRs should include:

- Description of the changes
- Why these changes are necessary
- Any potential side effects or areas of concern
- Screenshots if applicable (for UI changes)

## Coding Guidelines

### C++ Style Guide

We follow a consistent coding style to make the codebase maintainable:

- Use 4 spaces for indentation, not tabs
- Class and struct names use `PascalCase`
- Functions and methods use `camelCase`
- Variables use `camelCase`
- Constants and macros use `UPPER_SNAKE_CASE`
- Member variables use `camelCase` (no `m_` prefix)
- Always use curly braces for control structures, even for single-line statements
- Place opening braces on the same line for functions and control structures
- Add a space between control statements and opening parentheses (`if (condition)`, not `if(condition)`)
- Use `nullptr` instead of `NULL` or `0`
- Avoid using `using namespace std` in header files
- Include guards for all header files (using `#ifndef`/`#define`/`#endif`)

For easier formatting, we provide a `.clang-format` file in the repository. You can automatically format your code using:

```bash
clang-format -i path/to/your/file.cpp
```

To format all files in the project, you can run:

```bash
find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
```

Most modern IDEs and editors support clang-format integration, which allows automatic formatting on save or with keyboard shortcuts.

### Commit Messages

Write clear commit messages that explain the "what" and "why" of your changes:

- Use the imperative mood ("Add feature" not "Added feature" or "Adds feature")
- First line should be a concise summary (<50 characters)
- Followed by a blank line
- Optionally followed by a more detailed explanation
- Reference issue numbers if applicable

We follow this commit message convention: `type: short summary`

| Type     | Use for...                                      |
| -------- | ----------------------------------------------- |
| feat     | Adding a new feature (e.g., piping, prompt)     |
| fix      | Fixing a bug                                    |
| refactor | Code restructuring that doesn't change behavior |
| docs     | Updating the README or comments                 |
| style    | Formatting, spacing, indentation only           |
| test     | Adding/modifying tests                          |
| chore    | Build process, dependencies, misc               |

Example:

```text
feat: Add command history functionality

- Implement saving history to ~/.ninxsh_history
- Add retrieval of previous commands with up/down arrows
- Add history builtin command to display past commands

Fixes #42
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
│   └── utils.cpp       # Misc utilities
├── include/
│   ├── shell.hpp
│   ├── command.hpp
│   ├── executor.hpp
│   ├── builtins.hpp
│   └── util.hpp
├── Makefile
├── README.md
└── .gitignore
```

When adding new functionality:

- Keep files focused on a single responsibility
- Add new modules in pairs (implementation + header)
- Update the Makefile if adding new source files

## Testing

Currently, we test the shell manually. When submitting changes:

- Test all affected functionality
- Verify that your changes don't break existing features
- In your PR, list the test cases you've verified

## Communication

- GitHub Issues: For bug reports, feature requests, and public discussions
- Pull Requests: For code review and submitting changes

Please use GitHub Issues for most communications to keep discussions transparent and accessible to the entire community.

Thank you for contributing to ninxsh!
