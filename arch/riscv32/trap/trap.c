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
#include <arch/riscv32/include/trap.h>
#include <debug/debug_all.h>

extern void trap_entry(void);

// 异常处理初始化
void trap_init(void) {
    INFO("Initializing trap handling...");
    
    // 设置stvec寄存器指向异常处理入口 (S-mode)
    asm volatile("csrw stvec, %0" : : "r"(trap_entry));
    
    SUCCESS("Trap handling initialized");
}

// 主异常处理函数
void handle_trap(struct trap_frame *tf) {
    uint32_t cause = tf->scause;
    
    DEBUG("Trap occurred: cause=0x%08x, sepc=0x%08x, stval=0x%08x", cause, tf->sepc, tf->stval);
    
    if (cause & 0x80000000) {
        // 处理中断 (最高位为1表示中断)
        DEBUG("Handling interrupt: %d", cause & 0x7FFFFFFF);
        handle_interrupt(cause & 0x7FFFFFFF, tf);
    } else {
        // 处理异常 (最高位为0表示异常)
        DEBUG("Handling exception: %d", cause);
        switch (cause) {
            case EXCEPTION_INST_MISALIGNED:
                // 指令地址不对齐异常
                ERROR("Instruction address misaligned at 0x%08x", tf->sepc);
                PANIC("Instruction misaligned exception");
                break;
                
            case EXCEPTION_INST_ACCESS_FAULT:
                // 指令访问故障异常
                ERROR("Instruction access fault at 0x%08x", tf->sepc);
                PANIC("Instruction access fault");
                break;
                
            case EXCEPTION_ILLEGAL_INST:
                // 非法指令异常
                ERROR("Illegal instruction at 0x%08x, instruction: 0x%08x", tf->sepc, tf->stval);  // 看来ERROR宏还有问题，不支持传参。
                INFO("Attempting to skip illegal instruction...");
                tf->sepc += 4; // 尝试跳过非法指令（仅用于测试）
                WARN("Skipped illegal instruction - this is dangerous!");
                break;
                
            case EXCEPTION_BREAKPOINT:
                // 断点异常
                DEBUG("Breakpoint hit at 0x%08x", tf->sepc);
                // 跳过断点指令
                tf->sepc += 4;
                SUCCESS("Breakpoint exception handled");
                break;
                
            case EXCEPTION_LOAD_MISALIGNED:
                ERROR("Load address misaligned at 0x%08x, address: 0x%08x", tf->sepc, tf->stval);
                PANIC("Load misaligned exception");
                break;
                
            case EXCEPTION_LOAD_ACCESS_FAULT:
                ERROR("Load access fault at 0x%08x, address: 0x%08x", tf->sepc, tf->stval);
                INFO("Attempting to handle load access fault...");
                tf->sepc += 4; // 跳过出错指令（仅用于测试）
                WARN("Skipped faulting load instruction - this is dangerous!");
                break;
                
            case EXCEPTION_STORE_MISALIGNED:
                ERROR("Store address misaligned at 0x%08x, address: 0x%08x", tf->sepc, tf->stval);
                PANIC("Store misaligned exception");
                break;
                
            case EXCEPTION_STORE_ACCESS_FAULT:
                ERROR("Store access fault at 0x%08x, address: 0x%08x", tf->sepc, tf->stval);
                PANIC("Store access fault");
                break;
                
            case EXCEPTION_ECALL_FROM_U:
            case EXCEPTION_ECALL_FROM_S:
            case EXCEPTION_ECALL_FROM_M:
                DEBUG("System call from PC: 0x%08x", tf->sepc);
                // 系统调用处理
                syscall_handler(tf);
                break;
                
            case EXCEPTION_INST_PAGE_FAULT:
            case EXCEPTION_LOAD_PAGE_FAULT:
            case EXCEPTION_STORE_PAGE_FAULT:
                DEBUG("Page fault: cause=%d, addr=0x%08x", cause, tf->stval);
                // 页错误处理
                page_fault_handler(cause, tf->stval, tf);
                break;
                
            default:
                ERROR("Unknown exception: %d at 0x%08x", cause, tf->sepc);
                PANIC("Unhandled exception: %d at 0x%08x", cause, tf->sepc);
        }
    }
    
    DEBUG("Trap handling completed");
}

// 中断控制函数
void enable_interrupts(void) {
    asm volatile("csrsi sstatus, 0x2"); // 设置SIE位
}

void disable_interrupts(void) {
    asm volatile("csrci sstatus, 0x2"); // 清除SIE位
}

uint32_t save_and_disable_interrupts(void) {
    uint32_t old_status;
    asm volatile("csrr %0, sstatus" : "=r"(old_status));
    disable_interrupts();
    return old_status;
}

void restore_interrupts(uint32_t flags) {
    asm volatile("csrw sstatus, %0" : : "r"(flags));
}
