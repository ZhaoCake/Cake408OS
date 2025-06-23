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

static void print_number_with_width(unsigned long num, int base, int uppercase, int width, char pad_char) {
    char buf[32];
    int i = 0;
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    
    if (num == 0) {
        buf[i++] = '0';
    } else {
        while (num > 0) {
            buf[i++] = digits[num % base];
            num /= base;
        }
    }
    
    // 添加填充
    while (i < width) {
        buf[i++] = pad_char;
    }
    
    // 反向输出
    while (i > 0) {
        console_putchar(buf[--i]);
    }
}

static void print_number(unsigned long num, int base, int uppercase) {
    print_number_with_width(num, base, uppercase, 0, ' ');
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
            
            // 解析宽度和填充字符
            int width = 0;
            char pad_char = ' ';
            
            // 检查是否有填充字符指定
            if (*p == '0') {
                pad_char = '0';
                p++;
            }
            
            // 解析宽度
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                p++;
            }
            
            switch (*p) {
                case 'd':
                case 'i': {
                    int val = va_arg(args, int);
                    print_signed_number(val, 10);
                    break;
                }
                case 'u': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number_with_width(val, 10, 0, width, pad_char);
                    break;
                }
                case 'x': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number_with_width(val, 16, 0, width, pad_char);
                    break;
                }
                case 'X': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number_with_width(val, 16, 1, width, pad_char);
                    break;
                }
                case 'o': {
                    unsigned int val = va_arg(args, unsigned int);
                    print_number_with_width(val, 8, 0, width, pad_char);
                    break;
                }
                case 'p': {
                    void *ptr = va_arg(args, void*);
                    print_string("0x");
                    print_number_with_width((unsigned long)ptr, 16, 0, 8, '0');
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
                        print_number_with_width(val, 10, 0, width, pad_char);
                    } else if (*p == 'x') {
                        unsigned long val = va_arg(args, unsigned long);
                        print_number_with_width(val, 16, 0, width, pad_char);
                    } else if (*p == 'X') {
                        unsigned long val = va_arg(args, unsigned long);
                        print_number_with_width(val, 16, 1, width, pad_char);
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

int kputc(char c) {
    console_putchar(c);
    return (unsigned char)c;
}