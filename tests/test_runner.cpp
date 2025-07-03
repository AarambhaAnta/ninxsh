#include <iostream>

// Declaration of test functions
bool test_command_parsing();
bool test_builtin_commands();
bool test_executor();
bool test_utils();
bool test_signal_handling();
bool test_io_redirection();
bool test_pipeline();
bool test_dos_protection();  // Added for DoS protection tests
bool test_job_management();  // Added for job management tests
void runHistoryTests();      // Added for history tests

// Main test runner
int main() {
    int passed = 0;
    int total = 0;

// Helper macro to run tests and track results
#define RUN_TEST(test_func)                                                                        \
    std::cout << "Running " << #test_func << "... ";                                               \
    total++;                                                                                       \
    if (test_func()) {                                                                             \
        std::cout << "PASSED" << std::endl;                                                        \
        passed++;                                                                                  \
    } else {                                                                                       \
        std::cout << "FAILED" << std::endl;                                                        \
    }  // Run all tests
    RUN_TEST(test_command_parsing);
    RUN_TEST(test_builtin_commands);
    RUN_TEST(test_executor);
    RUN_TEST(test_utils);
    RUN_TEST(test_signal_handling);
    RUN_TEST(test_io_redirection);
    RUN_TEST(test_pipeline);
    RUN_TEST(test_dos_protection);
    RUN_TEST(test_job_management);

    // Run history tests (they use their own testing framework)
    std::cout << "Running history tests... " << std::endl;
    runHistoryTests();

    // Print summary
    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;

    return (passed == total) ? 0 : 1;
}
