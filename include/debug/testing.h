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
