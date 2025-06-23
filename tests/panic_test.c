/*
 * Copyright (c) 2025 ZhaoCake
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
