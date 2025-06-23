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

#ifndef _DEBUG_LOGGING_H
#define _DEBUG_LOGGING_H

#include <os/kprintf.h>
#include <debug/colors.h>

/* 通用日志输出宏 */
#define INFO(fmt, ...)    kprintf(COLOR_BRIGHT_BLUE "[INFO]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define WARN(fmt, ...)    kprintf(COLOR_YELLOW "[WARN]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define ERROR(fmt, ...)   kprintf(COLOR_RED "[ERROR]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define SUCCESS(fmt, ...) kprintf(COLOR_GREEN "[OK]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)

/* 带时间戳的日志宏（如果有时间系统的话） */
#define LOG_TRACE(fmt, ...) kprintf(COLOR_DIM "[TRACE]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  kprintf(COLOR_BRIGHT_BLUE "[INFO]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  kprintf(COLOR_YELLOW "[WARN]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) kprintf(COLOR_RED "[ERROR]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) kprintf(COLOR_BOLD COLOR_BG_RED "[FATAL]" COLOR_RESET " " fmt "\n", ##__VA_ARGS__)

#endif /* _DEBUG_LOGGING_H */
