#include "../include/os/types.h"

// UART 寄存器地址 (QEMU virt machine)
#define UART_BASE 0x10000000
#define UART_REG_TXFIFO   0x00
#define UART_REG_LSR      0x05

static volatile uint8_t *uart_base = (uint8_t*)UART_BASE;

void uart_putchar(char c) {
    // 等待发送FIFO有空间
    while ((uart_base[UART_REG_LSR] & 0x20) == 0);
    uart_base[UART_REG_TXFIFO] = c;
}

void uart_puts(const char *str) {
    while (*str) {
        if (*str == '\n') {
            uart_putchar('\r');
        }
        uart_putchar(*str++);
    }
}