# ninxsh Makefile for Linux
# Optimized for Linux development with GCC/Clang support

# Compiler configuration for Linux
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -I ../../include/
LDFLAGS = -pthread
DEBUGFLAGS = -g -O0 -DDEBUG -ggdb3
RELEASEFLAGS = -O3 -DNDEBUG -march=native -flto
SANITIZEFLAGS = -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer

# Linux specific flags
LINUX_FLAGS = -D_GNU_SOURCE
CXXFLAGS += $(LINUX_FLAGS)

# Project directories
SRCDIR = ../../src
INCDIR = ../../include
OBJDIR = ../../obj
BINDIR = ../../bin
TESTDIR = ../../tests

# Project files
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
BIN = ninxsh
TESTSRC = $(wildcard $(TESTDIR)/*.cpp)
TESTOBJ = $(patsubst $(TESTDIR)/%.cpp, $(OBJDIR)/%.o, $(TESTSRC))
TESTBIN = test_runner

# Linux distribution detection
DISTRO := $(shell lsb_release -si 2>/dev/null || echo "Unknown")
VERSION := $(shell lsb_release -sr 2>/dev/null || echo "Unknown")

# Package manager detection
PKG_MANAGER := $(shell which apt-get >/dev/null 2>&1 && echo "apt" || \
                which yum >/dev/null 2>&1 && echo "yum" || \
                which dnf >/dev/null 2>&1 && echo "dnf" || \
                which pacman >/dev/null 2>&1 && echo "pacman" || \
                which zypper >/dev/null 2>&1 && echo "zypper" || \
                echo "unknown")

# Default target
all: dirs $(BINDIR)/$(BIN)

# Debug build with GDB symbols
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all

# Release build with native optimizations
release: CXXFLAGS += $(RELEASEFLAGS)
release: all

# Sanitize build for comprehensive debugging
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

# Generate compile_commands.json for IDEs
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

# Valgrind memory testing (Linux specific)
valgrind: debug
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BINDIR)/$(BIN)

# GDB debugging session
gdb: debug
	gdb --args ./$(BINDIR)/$(BIN)

# Performance profiling with perf (Linux specific)
perf: release
	perf record -g ./$(BINDIR)/$(BIN)
	perf report

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR) $(BIN) $(SRCDIR)/*.o *.o
	rm -f perf.data*

# Clean everything including generated files
distclean: clean
	rm -f ../../compile_commands.json

# Format source code using clang-format
format:
	find $(SRCDIR) $(INCDIR) -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i

# Install dependencies based on package manager
deps:
	@echo "Detected Linux distribution: $(DISTRO) $(VERSION)"
	@echo "Package manager: $(PKG_MANAGER)"
	@case "$(PKG_MANAGER)" in \
		"apt") \
			echo "Installing dependencies with apt..."; \
			sudo apt-get update && sudo apt-get install -y build-essential clang-format bear valgrind gdb;; \
		"yum") \
			echo "Installing dependencies with yum..."; \
			sudo yum groupinstall -y "Development Tools" && sudo yum install -y clang-tools-extra bear valgrind gdb;; \
		"dnf") \
			echo "Installing dependencies with dnf..."; \
			sudo dnf groupinstall -y "Development Tools" && sudo dnf install -y clang-tools-extra bear valgrind gdb;; \
		"pacman") \
			echo "Installing dependencies with pacman..."; \
			sudo pacman -S --needed base-devel clang bear valgrind gdb;; \
		"zypper") \
			echo "Installing dependencies with zypper..."; \
			sudo zypper install -y -t pattern devel_basis && sudo zypper install -y clang-tools bear valgrind gdb;; \
		*) \
			echo "Unknown package manager. Please install manually:"; \
			echo "  - C++ compiler (g++ or clang++)"; \
			echo "  - build tools (make)"; \
			echo "  - clang-format"; \
			echo "  - bear (for compile_commands.json)"; \
			echo "  - valgrind (for memory debugging)"; \
			echo "  - gdb (for debugging)";; \
	esac

# Install to system directories
install: release
	sudo cp $(BINDIR)/$(BIN) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(BIN)

# Install to user directory
install-user: release
	mkdir -p $(HOME)/.local/bin
	cp $(BINDIR)/$(BIN) $(HOME)/.local/bin/
	chmod +x $(HOME)/.local/bin/$(BIN)
	@echo "Make sure $(HOME)/.local/bin is in your PATH"

# Create DEB package (Debian/Ubuntu)
deb: release
	mkdir -p $(BINDIR)/deb/usr/local/bin
	mkdir -p $(BINDIR)/deb/DEBIAN
	cp $(BINDIR)/$(BIN) $(BINDIR)/deb/usr/local/bin/
	echo "Package: ninxsh" > $(BINDIR)/deb/DEBIAN/control
	echo "Version: 1.0.0" >> $(BINDIR)/deb/DEBIAN/control
	echo "Architecture: amd64" >> $(BINDIR)/deb/DEBIAN/control
	echo "Maintainer: ninxsh developers" >> $(BINDIR)/deb/DEBIAN/control
	echo "Description: A lightweight Unix shell written in C++" >> $(BINDIR)/deb/DEBIAN/control
	dpkg-deb --build $(BINDIR)/deb $(BINDIR)/ninxsh.deb

# Create RPM package (Red Hat/Fedora/SUSE)
rpm: release
	mkdir -p $(HOME)/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
	cp $(BINDIR)/$(BIN) $(HOME)/rpmbuild/SOURCES/
	echo "Name: ninxsh" > $(HOME)/rpmbuild/SPECS/ninxsh.spec
	echo "Version: 1.0.0" >> $(HOME)/rpmbuild/SPECS/ninxsh.spec
	echo "Release: 1" >> $(HOME)/rpmbuild/SPECS/ninxsh.spec
	echo "Summary: A lightweight Unix shell" >> $(HOME)/rpmbuild/SPECS/ninxsh.spec
	echo "License: MIT" >> $(HOME)/rpmbuild/SPECS/ninxsh.spec
	echo "%description" >> $(HOME)/rpmbuild/SPECS/ninxsh.spec
	echo "A lightweight Unix shell written in C++" >> $(HOME)/rpmbuild/SPECS/ninxsh.spec
	echo "%files" >> $(HOME)/rpmbuild/SPECS/ninxsh.spec
	echo "/usr/local/bin/ninxsh" >> $(HOME)/rpmbuild/SPECS/ninxsh.spec
	rpmbuild -ba $(HOME)/rpmbuild/SPECS/ninxsh.spec

# System information
sysinfo:
	@echo "=== Linux System Information ==="
	@echo "Distribution: $(DISTRO) $(VERSION)"
	@echo "Kernel: $(shell uname -r)"
	@echo "Architecture: $(shell uname -m)"
	@echo "Package Manager: $(PKG_MANAGER)"
	@echo "Compiler: $(shell $(CXX) --version | head -n1)"
	@echo "CPU: $(shell nproc) cores"
	@echo "Memory: $(shell free -h | awk '/^Mem:/ {print $$2}')"

# Help target
help:
	@echo "ninxsh Makefile for Linux"
	@echo "========================="
	@echo "Available targets:"
	@echo "  all           - Build the shell (default)"
	@echo "  debug         - Build with debug info for GDB"
	@echo "  release       - Build with optimizations and LTO"
	@echo "  sanitize      - Build with sanitizers enabled"
	@echo "  static        - Build static binary for portability"
	@echo "  clean         - Remove build artifacts"
	@echo "  distclean     - Remove all generated files"
	@echo "  format        - Format source code with clang-format"
	@echo "  test          - Build and run tests"
	@echo "  run           - Build and run the shell"
	@echo "  install       - Install to /usr/local/bin (requires sudo)"
	@echo "  install-user  - Install to ~/.local/bin"
	@echo "  deps          - Install development dependencies"
	@echo "  compile_commands - Generate compile_commands.json"
	@echo ""
	@echo "Linux specific targets:"
	@echo "  valgrind      - Run with Valgrind memory checker"
	@echo "  gdb           - Start GDB debugging session"
	@echo "  perf          - Profile with Linux perf tools"
	@echo "  deb           - Create Debian package"
	@echo "  rpm           - Create RPM package"
	@echo "  sysinfo       - Show system information"
	@echo ""
	@echo "Current system: $(DISTRO) $(VERSION) ($(PKG_MANAGER))"

# Phony targets
.PHONY: all debug release sanitize static dirs clean distclean format test run install install-user deps valgrind gdb perf deb rpm sysinfo help compile_commands
