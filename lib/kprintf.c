#include "../include/os/types.h"
#include "../include/os/kprintf.h"

// 简单的printf实现
extern void uart_putchar(char c);
extern void uart_puts(const char *str);

static void print_number(uint32_t num, int base) {
    char buf[32];
    int i = 0;
    
    if (num == 0) {
        uart_putchar('0');
        return;
    }
    
    while (num > 0) {
        int digit = num % base;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        num /= base;
    }
    
    // 反向输出
    while (i > 0) {
        uart_putchar(buf[--i]);
    }
}

int kprintf(const char *fmt, ...) {
    const char *p = fmt;
    uint32_t *args = (uint32_t*)((char*)&fmt + sizeof(fmt));
    int arg_index = 0;
    
    while (*p) {
        if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
                case 'd':
                    print_number(args[arg_index++], 10);
                    break;
                case 'x':
                    print_number(args[arg_index++], 16);
                    break;
                case 's':
                    uart_puts("(string)"); // 简化实现，暂时显示占位符
                    arg_index++;
                    break;
                case 'c':
                    uart_putchar((char)args[arg_index++]);
                    break;
                default:
                    uart_putchar(*p);
                    break;
            }
        } else {
            uart_putchar(*p);
        }
        p++;
    }
    return 0;
}

int kputs(const char *str) {
    uart_puts(str);
    uart_putchar('\n');
    return 0;
}