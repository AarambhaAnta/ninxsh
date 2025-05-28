# ninxsh - A Custom Unix Shell

![LICENSE](https://img.shields.io/badge/license-MIT-green.svg)
![Last Commit](https://img.shields.io/github/last-commit/AarambhaAnta/ninxsh)
![Language](https://img.shields.io/github/languages/top/AarambhaAnta/ninxsh)
![Stars](https://img.shields.io/github/stars/AarambhaAnta/ninxsh?style=social)

`ninxsh` is a lightweight shell written in modern C++. It supports:

- REPL prompt
- Builtin Commands (`exit`, `echo`, `cd`, etc.)
- External executable support using `fork()` and `execvp()`
- Command history (planned)
- Quoting, redirection & pipelines (planned)

## Build Instructions

```bash
make        # To build ninxsh
./ninxsh    # To run ninxsh
make clean  # To delete the binary
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
├── Makefile
├── README.md
└── .gitignore
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
