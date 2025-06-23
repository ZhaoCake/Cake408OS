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

#ifndef _ARCH_RISCV32_TRAP_H
#define _ARCH_RISCV32_TRAP_H

#include <os/types.h>

// 寄存器保存的结构体
struct trap_frame {
    uint32_t regs[32];  // x0-x31 通用寄存器
    uint32_t sepc;      // 异常发生时的PC (S-mode)
    uint32_t sstatus;   // 状态寄存器 (S-mode)
    uint32_t scause;    // 异常原因 (S-mode)
    uint32_t stval;     // 异常值 (S-mode)
};

// 异常类型定义
#define EXCEPTION_INST_MISALIGNED    0  // 指令地址不对齐
#define EXCEPTION_INST_ACCESS_FAULT  1  // 指令访问故障
#define EXCEPTION_ILLEGAL_INST       2  // 非法指令
#define EXCEPTION_BREAKPOINT         3  // 断点
#define EXCEPTION_LOAD_MISALIGNED    4  // 加载地址不对齐
#define EXCEPTION_LOAD_ACCESS_FAULT  5  // 加载访问故障
#define EXCEPTION_STORE_MISALIGNED   6  // 存储地址不对齐
#define EXCEPTION_STORE_ACCESS_FAULT 7  // 存储访问故障
#define EXCEPTION_ECALL_FROM_U       8  // 用户模式系统调用
#define EXCEPTION_ECALL_FROM_S       9  // 监管模式系统调用
#define EXCEPTION_ECALL_FROM_M      11  // 机器模式系统调用
#define EXCEPTION_INST_PAGE_FAULT   12  // 指令页错误
#define EXCEPTION_LOAD_PAGE_FAULT   13  // 加载页错误
#define EXCEPTION_STORE_PAGE_FAULT  15  // 存储页错误

// 中断类型定义
#define INTERRUPT_S_SOFTWARE         1  // 监管模式软件中断
#define INTERRUPT_M_SOFTWARE         3  // 机器模式软件中断
#define INTERRUPT_S_TIMER            5  // 监管模式定时器中断
#define INTERRUPT_M_TIMER            7  // 机器模式定时器中断
#define INTERRUPT_S_EXTERNAL         9  // 监管模式外部中断
#define INTERRUPT_M_EXTERNAL        11  // 机器模式外部中断

// 函数声明
void trap_init(void);
void handle_trap(struct trap_frame *tf);
void handle_interrupt(uint32_t int_num, struct trap_frame *tf);
void syscall_handler(struct trap_frame *tf);
void page_fault_handler(uint32_t cause, uint32_t fault_addr, struct trap_frame *tf);

// 中断控制函数
void enable_interrupts(void);
void disable_interrupts(void);
uint32_t save_and_disable_interrupts(void);
void restore_interrupts(uint32_t flags);

#endif // _ARCH_RISCV32_TRAP_H
