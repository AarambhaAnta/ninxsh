# ninxsh - A Custom Unix Shell

ninxsh is a lightweight shell written in C++. It supports:

- REPL prompt
- Builtin Commands (`exit`, `echo`, `cd`, etc.)
- External executable support
- Command history
- Quoting, redirection & pipelines

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
