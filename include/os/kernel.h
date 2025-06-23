#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include <os/types.h>
#include <os/kprintf.h>
#include <utils/config.h>

/* 内核恐慌处理 */
#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        kprintf("PANIC: %s:%d: " fmt "\n",                                    \
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

/* 内核断言 */
#if CONFIG_DEBUG
#define ASSERT(condition)                                                      \
    do {                                                                       \
        if (!(condition)) {                                                    \
            PANIC("Assertion failed: %s", #condition);                        \
        }                                                                      \
    } while (0)
#else
#define ASSERT(condition) ((void)0)
#endif

/* 内核主函数声明 */
void kernel_main(void);

#endif /* _OS_KERNEL_H */