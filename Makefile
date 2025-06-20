# ninxsh Makefile - A lightweight Unix shell written in C++
# Configuration variables
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -I include/
LDFLAGS =
DEBUGFLAGS = -g -O0 -DDEBUG
RELEASEFLAGS = -O3 -DNDEBUG
SANITIZEFLAGS = -fsanitize=address -fsanitize=undefined

# Project directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
TESTDIR = tests

# Project files
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
BIN = ninxsh
TESTSRC = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJ = $(patsubst $(TESTDIR)/%.cpp, $(OBJDIR)/%.o, $(TESTSRC))
TESTBIN = test_runner

# Default target
all: dirs $(BINDIR)/$(BIN)

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all

# Release build
release: CXXFLAGS += $(RELEASEFLAGS)
release: all

# Sanitize build (for catching memory errors)
sanitize: CXXFLAGS += $(DEBUGFLAGS) $(SANITIZEFLAGS)
sanitize: LDFLAGS += $(SANITIZEFLAGS)
sanitize: all

# Create necessary directories
dirs:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Build the shell
$(BINDIR)/$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate compile_commands.json for IDE tooling
compile_commands: clean
	bear -- $(MAKE) all

# Run the shell
run: all
	./$(BINDIR)/$(BIN)

# Build and run tests
test: dirs $(TESTOBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BINDIR)/$(TESTBIN) $(TESTOBJ) $(filter-out $(OBJDIR)/main.o, $(OBJ))
	./$(BINDIR)/$(TESTBIN)

# Compile test files
$(OBJDIR)/%.o: $(TESTDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(BIN) src/*.o *.o

# Clean everything including generated files
distclean: clean
	rm -f compile_commands.json

# Format source code using clang-format
format:
	find $(SRCDIR) $(INCDIR) -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i

# Install the shell to /usr/local/bin (requires sudo)
install: release
	cp $(BINDIR)/$(BIN) /usr/local/bin/

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the shell (default)"
	@echo "  debug      - Build with debug information"
	@echo "  release    - Build with optimizations"
	@echo "  sanitize   - Build with sanitizers enabled"
	@echo "  clean      - Remove build artifacts"
	@echo "  distclean  - Remove all generated files"
	@echo "  format     - Format source code with clang-format"
	@echo "  test       - Build and run tests"
	@echo "  run        - Build and run the shell"
	@echo "  install    - Install the shell to /usr/local/bin"
	@echo "  compile_commands - Generate compile_commands.json for IDE tooling"

# Phony targets
.PHONY: all debug release sanitize dirs clean distclean format test run install help compile_commands
