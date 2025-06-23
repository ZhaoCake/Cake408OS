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

#ifndef _DEBUG_COLORS_H
#define _DEBUG_COLORS_H

#include <utils/config.h>

/* ANSI 颜色代码定义 */
#if CONFIG_COLOR_OUTPUT
#define COLOR_RESET     "\033[0m"
#define COLOR_BLACK     "\033[30m"
#define COLOR_RED       "\033[31m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_YELLOW    "\033[33m"
#define COLOR_BLUE      "\033[34m"
#define COLOR_MAGENTA   "\033[35m"
#define COLOR_CYAN      "\033[36m"
#define COLOR_WHITE     "\033[37m"

/* 亮色版本 */
#define COLOR_BRIGHT_BLACK   "\033[90m"
#define COLOR_BRIGHT_RED     "\033[91m"
#define COLOR_BRIGHT_GREEN   "\033[92m"
#define COLOR_BRIGHT_YELLOW  "\033[93m"
#define COLOR_BRIGHT_BLUE    "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN    "\033[96m"
#define COLOR_BRIGHT_WHITE   "\033[97m"

/* 背景色 */
#define COLOR_BG_RED         "\033[41m"
#define COLOR_BG_GREEN       "\033[42m"
#define COLOR_BG_YELLOW      "\033[43m"

/* 样式 */
#define COLOR_BOLD           "\033[1m"
#define COLOR_DIM            "\033[2m"
#define COLOR_UNDERLINE      "\033[4m"
#define COLOR_BLINK          "\033[5m"
#else
/* 如果禁用颜色输出，所有颜色代码都为空字符串 */
#define COLOR_RESET     ""
#define COLOR_BLACK     ""
#define COLOR_RED       ""
#define COLOR_GREEN     ""
#define COLOR_YELLOW    ""
#define COLOR_BLUE      ""
#define COLOR_MAGENTA   ""
#define COLOR_CYAN      ""
#define COLOR_WHITE     ""
#define COLOR_BRIGHT_BLACK   ""
#define COLOR_BRIGHT_RED     ""
#define COLOR_BRIGHT_GREEN   ""
#define COLOR_BRIGHT_YELLOW  ""
#define COLOR_BRIGHT_BLUE    ""
#define COLOR_BRIGHT_MAGENTA ""
#define COLOR_BRIGHT_CYAN    ""
#define COLOR_BRIGHT_WHITE   ""
#define COLOR_BG_RED         ""
#define COLOR_BG_GREEN       ""
#define COLOR_BG_YELLOW      ""
#define COLOR_BOLD           ""
#define COLOR_DIM            ""
#define COLOR_UNDERLINE      ""
#define COLOR_BLINK          ""
#endif

#endif /* _DEBUG_COLORS_H */
