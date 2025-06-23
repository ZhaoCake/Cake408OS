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
    
    /* 测试异常处理 */
    run_exception_tests();
    
    DEBUG("All tests completed successfully");
    TEST_END("All Tests Completed");
}
