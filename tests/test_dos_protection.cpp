#include <cassert>
#include <iostream>
#include <string>

#include "command.hpp"
#include "limits.hpp"
#include "utils.hpp"

bool test_dos_protection() {
    bool allTestsPassed = true;

    // Test 1: Very long input should be rejected by parseCommand
    {
        std::string longInput(ninxsh::limits::TEST_LONG_INPUT, 'A');  // 8000 character string
        ParsedCommand result = parseCommand(longInput);

        if (!result.hasError || result.errorMessage.find("too long") == std::string::npos) {
            std::cerr << "Failed DoS protection test: parseCommand should reject very long input "
                         "with error flag"
                      << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 2: Very long path should be handled gracefully by expandPath
    {
        std::string longPath(ninxsh::limits::TEST_LONG_PATH, 'A');  // 4000 character path
        std::string result = expandPath(longPath);

        // Should return the original path unchanged (no crash/hang)
        if (result != longPath) {
            std::cerr
                << "Failed DoS protection test: expandPath should handle long paths gracefully"
                << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 3: Very long string should be handled gracefully by expandEnvVars
    {
        std::string longString(ninxsh::limits::TEST_LONG_PATH, 'A');  // 4000 character string
        std::string result = expandEnvVars(longString);

        // Should return the original string unchanged (no crash/hang)
        if (result != longString) {
            std::cerr
                << "Failed DoS protection test: expandEnvVars should handle long strings gracefully"
                << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 4: Normal length input should still work
    {
        std::string normalInput = "echo hello world";
        ParsedCommand result = parseCommand(normalInput);

        if (result.hasError || result.pipeline.empty() || result.pipeline[0].args.empty()) {
            std::cerr << "Failed DoS protection test: normal input should still be parsed correctly"
                      << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 5: String with no environment variables should be fast-tracked
    {
        std::string noEnvVars = "this/is/a/normal/path/without/env/vars";
        std::string result = expandPath(noEnvVars);

        if (result != noEnvVars) {
            std::cerr << "Failed DoS protection test: paths without env vars should be unchanged"
                      << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 6: Edge case - input at exactly the limit should be accepted
    {
        std::string limitInput(ninxsh::limits::MAX_INPUT_LENGTH, 'A');  // Exactly at limit
        ParsedCommand result = parseCommand(limitInput);

        if (result.hasError) {
            std::cerr << "Failed DoS protection test: input at limit should be accepted"
                      << std::endl;
            allTestsPassed = false;
        }
    }

    // Test 7: Edge case - input just over limit should be rejected
    {
        std::string overLimitInput(ninxsh::limits::TEST_OVER_LIMIT, 'A');  // Just over limit
        ParsedCommand result = parseCommand(overLimitInput);

        if (!result.hasError || result.errorMessage.find("too long") == std::string::npos) {
            std::cerr << "Failed DoS protection test: input over limit should be rejected"
                      << std::endl;
            allTestsPassed = false;
        }
    }

    return allTestsPassed;
}
