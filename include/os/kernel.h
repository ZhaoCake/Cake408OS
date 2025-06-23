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