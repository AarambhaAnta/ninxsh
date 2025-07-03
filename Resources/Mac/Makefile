# ninxsh Makefile for macOS
# Optimized for macOS development environment with Xcode toolchain

# Compiler configuration for macOS
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -I ../../include/
LDFLAGS = 
DEBUGFLAGS = -g -O0 -DDEBUG -fstandalone-debug
RELEASEFLAGS = -O3 -DNDEBUG -flto
SANITIZEFLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer

# macOS specific flags
MACOS_FLAGS = -mmacosx-version-min=10.15
CXXFLAGS += $(MACOS_FLAGS)

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

# macOS specific tools
DSYM_UTIL = dsymutil
CODESIGN = codesign

# Default target
all: dirs $(BINDIR)/$(BIN)

# Debug build with debug symbols for lldb
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all
debug: dsym

# Release build with Link Time Optimization
release: CXXFLAGS += $(RELEASEFLAGS)
release: all

# Sanitize build for memory debugging
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

# Generate debug symbols for lldb (macOS specific)
dsym: $(BINDIR)/$(BIN)
	$(DSYM_UTIL) $(BINDIR)/$(BIN)

# Generate compile_commands.json for Xcode/CLion
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
	rm -rf $(OBJDIR) $(BINDIR) $(BIN) $(SRCDIR)/*.o *.o
	rm -rf $(BINDIR)/$(BIN).dSYM

# Clean everything including generated files
distclean: clean
	rm -f ../../compile_commands.json

# Format source code using clang-format (Xcode style)
format:
	find $(SRCDIR) $(INCDIR) -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i -style="{BasedOnStyle: llvm, IndentWidth: 4, ColumnLimit: 100}"

# Install using Homebrew prefix
install: release
	cp $(BINDIR)/$(BIN) /usr/local/bin/

# Install for Apple Silicon Macs
install-arm64: release
	cp $(BINDIR)/$(BIN) /opt/homebrew/bin/

# Code signing for distribution (optional)
codesign: release
	$(CODESIGN) -s "Developer ID Application" $(BINDIR)/$(BIN)

# Create macOS app bundle (optional)
bundle: release
	mkdir -p $(BINDIR)/ninxsh.app/Contents/MacOS
	cp $(BINDIR)/$(BIN) $(BINDIR)/ninxsh.app/Contents/MacOS/
	echo '<?xml version="1.0" encoding="UTF-8"?>' > $(BINDIR)/ninxsh.app/Contents/Info.plist
	echo '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' >> $(BINDIR)/ninxsh.app/Contents/Info.plist
	echo '<plist version="1.0"><dict>' >> $(BINDIR)/ninxsh.app/Contents/Info.plist
	echo '<key>CFBundleExecutable</key><string>ninxsh</string>' >> $(BINDIR)/ninxsh.app/Contents/Info.plist
	echo '<key>CFBundleIdentifier</key><string>com.developer.ninxsh</string>' >> $(BINDIR)/ninxsh.app/Contents/Info.plist
	echo '</dict></plist>' >> $(BINDIR)/ninxsh.app/Contents/Info.plist

# Xcode project generation (requires cmake)
xcode:
	@echo "To generate Xcode project, use: cmake -G Xcode .."

# Help target
help:
	@echo "ninxsh Makefile for macOS"
	@echo "=========================="
	@echo "Available targets:"
	@echo "  all           - Build the shell (default)"
	@echo "  debug         - Build with debug info for lldb"
	@echo "  release       - Build with optimizations and LTO"
	@echo "  sanitize      - Build with sanitizers enabled"
	@echo "  clean         - Remove build artifacts"
	@echo "  distclean     - Remove all generated files"
	@echo "  format        - Format source code with clang-format"
	@echo "  test          - Build and run tests"
	@echo "  run           - Build and run the shell"
	@echo "  install       - Install to /usr/local/bin"
	@echo "  install-arm64 - Install to /opt/homebrew/bin (Apple Silicon)"
	@echo "  dsym          - Generate debug symbols for lldb"
	@echo "  codesign      - Sign the binary for distribution"
	@echo "  bundle        - Create macOS app bundle"
	@echo "  compile_commands - Generate compile_commands.json"
	@echo ""
	@echo "macOS specific features:"
	@echo "  - Optimized for clang/LLVM toolchain"
	@echo "  - Debug symbols for lldb debugging"
	@echo "  - Link Time Optimization for release builds"
	@echo "  - Code signing support"
	@echo "  - App bundle creation"

# Phony targets
.PHONY: all debug release sanitize dirs clean distclean format test run install install-arm64 dsym codesign bundle xcode help compile_commands
