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

#ifndef _DEBUG_DEBUG_H
#define _DEBUG_DEBUG_H

#include <os/kprintf.h>
#include <utils/config.h>
#include <debug/colors.h>

/* 调试输出宏 */
#if CONFIG_DEBUG
#define DEBUG(fmt, ...)                                                        \
    kprintf(COLOR_DIM COLOR_CYAN "[DEBUG]" COLOR_RESET COLOR_DIM " %s:%d: "   \
           fmt COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(fmt, ...) do { } while (0)
#endif

/* 详细启动信息宏 */
#if CONFIG_VERBOSE_BOOT
#define BOOT_INFO(fmt, ...) kprintf(COLOR_BLUE fmt COLOR_RESET, ##__VA_ARGS__)
#else
#define BOOT_INFO(fmt, ...) do { } while (0)
#endif

#endif /* _DEBUG_DEBUG_H */
