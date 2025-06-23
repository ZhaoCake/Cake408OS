#ifndef _DEBUG_PANIC_H
#define _DEBUG_PANIC_H

#include <os/kernel.h>
#include <debug/colors.h>
#include <utils/config.h>

/* 调试版本的彩色 PANIC 宏 - 仅用于调试时的增强显示 */
#if CONFIG_COLOR_OUTPUT && CONFIG_DEBUG
#define DEBUG_PANIC(fmt, ...)                                                  \
    do {                                                                       \
        kprintf(COLOR_BOLD COLOR_BG_RED COLOR_WHITE "PANIC" COLOR_RESET        \
               COLOR_RED ": %s:%d: " fmt COLOR_RESET "\n",                    \
               __FILE__, __LINE__, ##__VA_ARGS__);                            \
        if (CONFIG_PANIC_HALT) {                                               \
            while (1) {                                                        \
                asm volatile("wfi"); /* Wait For Interrupt */                 \
            }                                                                  \
        } else {                                                               \
            /* 这里可以添加系统重启逻辑 */                                        \
            while (1) {                                                        \
                asm volatile("wfi");                                           \
            }                                                                  \
        }                                                                      \
    } while (0)

/* 调试版本的彩色断言宏 */
#define DEBUG_ASSERT(condition)                                                \
    do {                                                                       \
        if (!(condition)) {                                                    \
            DEBUG_PANIC("Assertion failed: %s", #condition);                  \
        }                                                                      \
    } while (0)
#else
#define DEBUG_PANIC(fmt, ...) PANIC(fmt, ##__VA_ARGS__)
#define DEBUG_ASSERT(condition) ASSERT(condition)
#endif

#endif /* _DEBUG_PANIC_H */
