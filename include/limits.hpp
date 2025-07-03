#ifndef LIMITS_HPP
#define LIMITS_HPP

#include <cstddef>

namespace ninxsh {
namespace limits {

// DoS Protection Limits
constexpr size_t MAX_INPUT_LENGTH = 4096;   // Maximum command line input length
constexpr size_t MAX_PATH_LENGTH = 2048;    // Maximum file path length
constexpr size_t MAX_STRING_LENGTH = 2048;  // Maximum string length for expansions

// Test Constants (based on limits above)
constexpr size_t TEST_LONG_INPUT = 8000;                  // Test input longer than MAX_INPUT_LENGTH
constexpr size_t TEST_LONG_PATH = 4000;                   // Test path longer than MAX_PATH_LENGTH
constexpr size_t TEST_OVER_LIMIT = MAX_INPUT_LENGTH + 1;  // Just over the input limit

}  // namespace limits
}  // namespace ninxsh

#endif  // LIMITS_HPP
