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

#ifndef _DEBUG_TESTING_H
#define _DEBUG_TESTING_H

#include <os/kprintf.h>
#include <debug/colors.h>

/* 测试相关的彩色输出宏 */
#define TEST_START(name)    kprintf(COLOR_CYAN "=== " name " ===" COLOR_RESET "\n")
#define TEST_END(name)      kprintf(COLOR_CYAN "=== " name " Complete ===" COLOR_RESET "\n")
#define TEST_PASS(fmt, ...) kprintf(COLOR_GREEN "✓ " fmt COLOR_RESET "\n", ##__VA_ARGS__)
#define TEST_FAIL(fmt, ...) kprintf(COLOR_RED "✗ " fmt COLOR_RESET "\n", ##__VA_ARGS__)
#define TEST_SKIP(fmt, ...) kprintf(COLOR_YELLOW "⚬ " fmt COLOR_RESET "\n", ##__VA_ARGS__)

/* 测试统计和结果宏 */
#define TEST_SECTION(name)  kprintf(COLOR_BOLD COLOR_UNDERLINE name COLOR_RESET "\n")
#define TEST_SUMMARY(passed, total) \
    kprintf(COLOR_BOLD "Test Summary: %d/%d passed" COLOR_RESET "\n", passed, total)

/* 测试断言宏 */
#define TEST_ASSERT(condition, desc) \
    do { \
        if (condition) { \
            TEST_PASS(desc); \
        } else { \
            TEST_FAIL(desc); \
        } \
    } while (0)

#define TEST_ASSERT_EQ(expected, actual, desc) \
    do { \
        if ((expected) == (actual)) { \
            TEST_PASS(desc " (expected: %d, actual: %d)", (int)(expected), (int)(actual)); \
        } else { \
            TEST_FAIL(desc " (expected: %d, actual: %d)", (int)(expected), (int)(actual)); \
        } \
    } while (0)

#define TEST_ASSERT_STR_EQ(expected, actual, desc) \
    do { \
        if (strcmp((expected), (actual)) == 0) { \
            TEST_PASS(desc); \
        } else { \
            TEST_FAIL(desc " (expected: \"%s\", actual: \"%s\")", (expected), (actual)); \
        } \
    } while (0)

#endif /* _DEBUG_TESTING_H */
