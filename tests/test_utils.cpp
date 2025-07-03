#include <cassert>
#include <iostream>
#include <string>

#include "utils.hpp"

bool test_utils() {
    bool allTestsPassed = true;

    // Test 1: Path expansion for tilde (~)
    {
        std::string input = "~/documents";
        std::string expanded = expandPath(input);
        std::string homeDir = getenv("HOME") ? getenv("HOME") : "";

        if (homeDir.empty()) {
            std::cerr << "HOME environment variable not set, skipping test" << std::endl;
        } else {
            std::string expected = homeDir + "/documents";
            if (expanded != expected) {
                std::cerr << "Failed tilde expansion test" << std::endl;
                std::cerr << "  Expected: " << expected << std::endl;
                std::cerr << "  Got: " << expanded << std::endl;
                allTestsPassed = false;
            }
        }
    }

    // Test 2: Environment variable expansion
    {
        std::string input = "Hello $USER";
        std::string expanded = expandEnvVars(input);
        std::string username = getenv("USER") ? getenv("USER") : "";

        if (username.empty()) {
            std::cerr << "USER environment variable not set, skipping test" << std::endl;
        } else {
            std::string expected = "Hello " + username;
            if (expanded != expected) {
                std::cerr << "Failed environment variable expansion test" << std::endl;
                std::cerr << "  Expected: " << expected << std::endl;
                std::cerr << "  Got: " << expanded << std::endl;
                allTestsPassed = false;
            }
        }
    }

    // Test 3: String tokenization
    {
        std::string input = "This is a test";
        std::vector<std::string> tokens = tokenize(input, ' ');

        std::vector<std::string> expected = {"This", "is", "a", "test"};
        if (tokens != expected) {
            std::cerr << "Failed string tokenization test" << std::endl;
            std::cerr << "  Expected: ";
            for (const auto& token : expected)
                std::cerr << token << " ";
            std::cerr << std::endl << "  Got: ";
            for (const auto& token : tokens)
                std::cerr << token << " ";
            std::cerr << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 4: String trimming
    {
        std::string input = "  whitespace  ";
        std::string trimmed = trim(input);

        std::string expected = "whitespace";
        if (trimmed != expected) {
            std::cerr << "Failed string trimming test" << std::endl;
            std::cerr << "  Expected: '" << expected << "'" << std::endl;
            std::cerr << "  Got: '" << trimmed << "'" << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 5: Terminal utility functions
    {
        try {
            // Test username function
            std::string username = getCurrentUsername();
            if (username.empty()) {
                std::cerr << "Failed username test - empty result" << std::endl;
                allTestsPassed = false;
            }

            // Test hostname function
            std::string hostname = getCurrentHostname();
            if (hostname.empty()) {
                std::cerr << "Failed hostname test - empty result" << std::endl;
                allTestsPassed = false;
            }

            // Test current working directory function
            std::string cwd = getCurrentWorkingDir();
            if (cwd.empty()) {
                std::cerr << "Failed cwd test - empty result" << std::endl;
                allTestsPassed = false;
            }

            // Test colored prompt generation
            std::string prompt = getColoredPrompt();
            if (prompt.empty()) {
                std::cerr << "Failed prompt test - empty result" << std::endl;
                allTestsPassed = false;
            }

            // Test terminal detection function
            bool isTerminal = isOutputToTerminal();
            (void)isTerminal;  // Function should not crash
        } catch (const std::exception& e) {
            std::cerr << "Failed terminal utility tests - exception: " << e.what() << std::endl;
            allTestsPassed = false;
        }
    }

    return allTestsPassed;
}
