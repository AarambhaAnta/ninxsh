# ninxsh - A Custom Unix Shell

![LICENSE](https://img.shields.io/badge/license-MIT-green.svg)
![Last Commit](https://img.shields.io/github/last-commit/AarambhaAnta/ninxsh)
![Language](https://img.shields.io/github/languages/top/AarambhaAnta/ninxsh)
![Stars](https://img.shields.io/github/stars/AarambhaAnta/ninxsh?style=social)

`ninxsh` is a lightweight shell written in modern C++. It supports:

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
