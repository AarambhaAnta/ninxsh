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

---

## Deliverables at the End of **Day 1**

- [x] Project compiles with `make`
- [x] **Print Prompt:** `ninxsh >`
- [x] Reads user input
- [x] Gracefully handles empty input and `Ctrl + C` (EOF)
- [x] Initial commits in Git

## Deliverables at the End of **Day 2**

- [x] Run external commands using `fork()` and `execvp()`
- [ ] Supports basic Unix commands like `ls`, `pwd`, `echo`, `date`, etc.
- [ ] Basic error handling for invalid commands (shows "command not found")
- [ ] Robust input tokenization and conversion to C-style argument arrays
- [ ] Handles command execution filure gracefully without crashing
- [ ] Proper memory management with `strdup()` and `free()`