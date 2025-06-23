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
#include <os/stdarg.h>
#include <os/string.h>
#include <sbi.h>

// 使用SBI接口进行控制台输出
static void console_putchar(char c) {
    sbi_console_putchar(c);
}

static void print_string(const char *str) {
    if (str) {
        while (*str) {
            console_putchar(*str++);
        }
    } else {
        print_string("(null)");
    }
}

static void print_number(unsigned long num, int base, int uppercase) {
    char buf[32];
    int i = 0;
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    
    if (num == 0) {
        console_putchar('0');
        return;
    }
    
    while (num > 0) {
        buf[i++] = digits[num % base];
        num /= base;
    }
    
    // 反向输出
    while (i > 0) {
        console_putchar(buf[--i]);
    }
}

static void print_signed_number(long num, int base) {
    if (num < 0) {
        console_putchar('-');
        num = -num;
    }
    print_number((unsigned long)num, base, 0);
}

int kvprintf(const char *fmt, va_list args) {
    const char *p = fmt;
    int count = 0;
    
    while (*p) {
        if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
                case 'd':
                case 'i': {
                    int val = va_arg(args, int);
                    print_signed_number(val, 10);
                    break;
                }
                case 'u': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number(val, 10, 0);
                    break;
                }
                case 'x': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number(val, 16, 0);
                    break;
                }
                case 'X': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number(val, 16, 1);
                    break;
                }
                case 'o': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number(val, 8, 0);
                    break;
                }
                case 'p': {
                    void *ptr = va_arg(args, void*);
                    print_string("0x");
                    print_number((unsigned long)ptr, 16, 0);
                    break;
                }
                case 's': {
                    const char *str = va_arg(args, const char*);
                    print_string(str);
                    break;
                }
                case 'c': {
                    int ch = va_arg(args, int);
                    console_putchar((char)ch);
                    break;
                }
                case 'l': {
                    p++;
                    if (*p == 'd' || *p == 'i') {
                        long val = va_arg(args, long);
                        print_signed_number(val, 10);
                    } else if (*p == 'u') {
                        unsigned long val = va_arg(args, unsigned long);
                        print_number(val, 10, 0);
                    } else if (*p == 'x') {
                        unsigned long val = va_arg(args, unsigned long);
                        print_number(val, 16, 0);
                    } else if (*p == 'X') {
                        unsigned long val = va_arg(args, unsigned long);
                        print_number(val, 16, 1);
                    } else {
                        console_putchar('%');
                        console_putchar('l');
                        console_putchar(*p);
                    }
                    break;
                }
                case '%':
                    console_putchar('%');
                    break;
                default:
                    console_putchar('%');
                    console_putchar(*p);
                    break;
            }
        } else {
            console_putchar(*p);
        }
        p++;
        count++;
    }
    return count;
}

int kprintf(const char *fmt, ...) {
    va_list args;
    int result;
    
    va_start(args, fmt);
    result = kvprintf(fmt, args);
    va_end(args);
    
    return result;
}

int kputs(const char *str) {
    while (*str) {
        if (*str == '\n') {
            console_putchar('\r');
        }
        console_putchar(*str++);
    }
    console_putchar('\n');
    return 0;
}