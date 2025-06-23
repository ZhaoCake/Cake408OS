#include <os/types.h>
#include <os/kprintf.h>
#include <os/kernel.h>
#include <debug/debug_all.h>
#include "tests.h"

void run_all_tests(void) {
    DEBUG("Test runner started");
    TEST_START("Running System Tests");
    
#if CONFIG_DEBUG
    INFO("Running tests in debug mode...");
#endif
    
    /* 测试内核恐慌功能 */
    test_panic_feature();
    
    /* 测试SBI接口 */
    sbi_test();
    
    DEBUG("All tests completed successfully");
    TEST_END("All Tests Completed");
}
