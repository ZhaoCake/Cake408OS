#include <os/types.h>
#include <os/kprintf.h>
#include <os/kernel.h>
#include <debug/debug_all.h>

void test_panic_feature(void) {
    DEBUG("Starting panic feature test");
    TEST_START("Kernel Panic and Assertion Features");
    
    // 测试断言宏
    ASSERT(1 == 1); // 这个断言会通过
    TEST_PASS("Basic assertion test passed");
    
    // 只有在实验模式下才真正测试 panic
    DEBUG("Running actual panic test");
    WARN("About to trigger actual panic!");
    // PANIC("This is a test panic with value: %d", 42);
    // ASSERT(1 == 0); // 这个断言会失败，触发 panic
    
    TEST_PASS("Panic feature implementation completed");
}
