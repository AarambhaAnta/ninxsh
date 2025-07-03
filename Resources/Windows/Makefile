# ninxsh Makefile for Windows
# Optimized for MinGW-w64/MSYS2 development environment

# Compiler configuration for Windows
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -I ../../include/
LDFLAGS = -static-libgcc -static-libstdc++
DEBUGFLAGS = -g -O0 -DDEBUG
RELEASEFLAGS = -O3 -DNDEBUG -s
SANITIZEFLAGS = -fsanitize=address -fsanitize=undefined

# Windows specific flags
WINDOWS_FLAGS = -D_WIN32_WINNT=0x0601 -DWINVER=0x0601 -D_GNU_SOURCE
CXXFLAGS += $(WINDOWS_FLAGS)

# Windows executable extension
EXE_EXT = .exe

# Project directories (Windows path style)
SRCDIR = ../../src
INCDIR = ../../include
OBJDIR = ../../obj
BINDIR = ../../bin
TESTDIR = ../../tests

# Project files
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
BIN = ninxsh$(EXE_EXT)
TESTSRC = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJ = $(patsubst $(TESTDIR)/%.cpp, $(OBJDIR)/%.o, $(TESTSRC))
TESTBIN = test_runner$(EXE_EXT)

# Windows environment detection
MSYS := $(shell echo $$MSYSTEM 2>/dev/null)
PWD_WIN := $(shell pwd -W 2>/dev/null || pwd)

# Default target
all: dirs $(BINDIR)/$(BIN)

# Debug build
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all

# Release build with stripping
release: CXXFLAGS += $(RELEASEFLAGS)
release: all

# Sanitize build (limited support on Windows)
sanitize: CXXFLAGS += $(DEBUGFLAGS) $(SANITIZEFLAGS)
sanitize: LDFLAGS += $(SANITIZEFLAGS)
sanitize: all

# Static build for portability
static: LDFLAGS += -static
static: release

# Create necessary directories
dirs:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Build the shell
$(BINDIR)/$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate compile_commands.json (if bear is available)
compile_commands: clean
	@if command -v bear >/dev/null 2>&1; then \
		bear -- $(MAKE) all; \
	else \
		echo "Bear not available. Install with: pacman -S bear"; \
	fi

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
	rm -rf $(OBJDIR) $(BINDIR) $(BIN) $(SRCDIR)/*.o *.o

# Clean everything including generated files
distclean: clean
	rm -f ../../compile_commands.json

# Format source code using clang-format (if available)
format:
	@if command -v clang-format >/dev/null 2>&1; then \
		find $(SRCDIR) $(INCDIR) -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i; \
	else \
		echo "clang-format not available. Install with: pacman -S clang-tools-extra"; \
	fi

# Install MSYS2 dependencies
deps:
	@echo "Installing dependencies for MSYS2..."
	@if command -v pacman >/dev/null 2>&1; then \
		pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-clang-tools-extra bear make; \
	else \
		echo "This target requires MSYS2. Please install manually:"; \
		echo "  - MinGW-w64 GCC"; \
		echo "  - clang-tools-extra (for clang-format)"; \
		echo "  - bear (for compile_commands.json)"; \
		echo "  - make"; \
	fi

# Install to Windows system directory (requires admin)
install: release
	@echo "Installing to C:/Windows/System32/ (requires administrator privileges)"
	@if [ -w "/c/Windows/System32" ]; then \
		cp $(BINDIR)/$(BIN) /c/Windows/System32/; \
		echo "Installed successfully"; \
	else \
		echo "Run as administrator or use install-user target"; \
	fi

# Install to user directory
install-user: release
	@mkdir -p $(HOME)/bin
	cp $(BINDIR)/$(BIN) $(HOME)/bin/
	@echo "Installed to $(HOME)/bin/"
	@echo "Make sure $(HOME)/bin is in your PATH"

# Create Windows installer using NSIS (if available)
installer: release
	@if command -v makensis >/dev/null 2>&1; then \
		echo "Creating Windows installer..."; \
		mkdir -p $(BINDIR)/installer; \
		echo '!include "MUI2.nsh"' > $(BINDIR)/installer/ninxsh.nsi; \
		echo 'Name "ninxsh"' >> $(BINDIR)/installer/ninxsh.nsi; \
		echo 'OutFile "ninxsh-installer.exe"' >> $(BINDIR)/installer/ninxsh.nsi; \
		echo 'InstallDir "$$PROGRAMFILES\\ninxsh"' >> $(BINDIR)/installer/ninxsh.nsi; \
		echo 'Section' >> $(BINDIR)/installer/ninxsh.nsi; \
		echo '  SetOutPath $$INSTDIR' >> $(BINDIR)/installer/ninxsh.nsi; \
		echo '  File "$(PWD_WIN)/$(BINDIR)/$(BIN)"' >> $(BINDIR)/installer/ninxsh.nsi; \
		echo 'SectionEnd' >> $(BINDIR)/installer/ninxsh.nsi; \
		cd $(BINDIR)/installer && makensis ninxsh.nsi; \
	else \
		echo "NSIS not available. Install NSIS to create Windows installer."; \
	fi

# Create portable ZIP package
portable: release
	@mkdir -p $(BINDIR)/portable
	cp $(BINDIR)/$(BIN) $(BINDIR)/portable/
	cp ../../README.md $(BINDIR)/portable/
	cp ../../LICENSE $(BINDIR)/portable/
	echo "@echo off" > $(BINDIR)/portable/run.bat
	echo "$(BIN)" >> $(BINDIR)/portable/run.bat
	cd $(BINDIR) && zip -r ninxsh-portable.zip portable/

# Debug with GDB (if available)
gdb: debug
	@if command -v gdb >/dev/null 2>&1; then \
		gdb --args ./$(BINDIR)/$(BIN); \
	else \
		echo "GDB not available. Install with: pacman -S gdb"; \
	fi

# Windows environment information
wininfo:
	@echo "=== Windows Environment Information ==="
	@echo "MSYS Environment: $(MSYS)"
	@echo "Working Directory: $(PWD_WIN)"
	@echo "Compiler: $(shell $(CXX) --version | head -n1)"
	@echo "Windows Version: $(shell cmd //c ver 2>/dev/null || echo 'Unknown')"
	@echo "Architecture: $(shell uname -m)"
	@echo "Available Tools:"
	@echo "  GCC: $(shell command -v gcc >/dev/null 2>&1 && echo 'Yes' || echo 'No')"
	@echo "  Clang-format: $(shell command -v clang-format >/dev/null 2>&1 && echo 'Yes' || echo 'No')"
	@echo "  Bear: $(shell command -v bear >/dev/null 2>&1 && echo 'Yes' || echo 'No')"
	@echo "  GDB: $(shell command -v gdb >/dev/null 2>&1 && echo 'Yes' || echo 'No')"
	@echo "  NSIS: $(shell command -v makensis >/dev/null 2>&1 && echo 'Yes' || echo 'No')"

# Help target
help:
	@echo "ninxsh Makefile for Windows (MinGW/MSYS2)"
	@echo "=========================================="
	@echo "Available targets:"
	@echo "  all           - Build the shell (default)"
	@echo "  debug         - Build with debug information"
	@echo "  release       - Build with optimizations"
	@echo "  sanitize      - Build with sanitizers (limited support)"
	@echo "  static        - Build static binary"
	@echo "  clean         - Remove build artifacts"
	@echo "  distclean     - Remove all generated files"
	@echo "  format        - Format source code (requires clang-format)"
	@echo "  test          - Build and run tests"
	@echo "  run           - Build and run the shell"
	@echo "  install       - Install to system (requires admin)"
	@echo "  install-user  - Install to user directory"
	@echo "  deps          - Install MSYS2 dependencies"
	@echo "  compile_commands - Generate compile_commands.json (requires bear)"
	@echo ""
	@echo "Windows specific targets:"
	@echo "  installer     - Create NSIS installer (requires NSIS)"
	@echo "  portable      - Create portable ZIP package"
	@echo "  gdb           - Debug with GDB (requires GDB)"
	@echo "  wininfo       - Show Windows environment information"
	@echo ""
	@echo "Requirements:"
	@echo "  - MinGW-w64 or MSYS2"
	@echo "  - GCC compiler"
	@echo "  - Optional: clang-format, bear, gdb, NSIS"
	@echo ""
	@echo "Current environment: $(MSYS)"

# Phony targets
.PHONY: all debug release sanitize static dirs clean distclean format test run install install-user deps installer portable gdb wininfo help compile_commands
