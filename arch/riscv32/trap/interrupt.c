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

static volatile uint32_t timer_ticks = 0;

// 定时器中断处理
static void timer_interrupt_handler(struct trap_frame *tf) {
    timer_ticks++;
    
    // 每1000次定时器中断打印一次信息（用于调试）
    if (timer_ticks % 1000 == 0) {
        DEBUG("Timer tick: %d", timer_ticks);
    }
    
    // 清除定时器中断pending位
    // 注意：在真实硬件上可能需要向CLINT写入下一次中断时间
    // 这里暂时简单处理
}

// 软件中断处理
static void software_interrupt_handler(struct trap_frame *tf) {
    DEBUG("Software interrupt received");
    // 清除软件中断pending位
    asm volatile("csrci mip, 0x8"); // 清除MSIP位
}

// 外部中断处理
static void external_interrupt_handler(struct trap_frame *tf) {
    DEBUG("External interrupt received");
    
    // 这里应该查询PLIC (Platform-Level Interrupt Controller) 
    // 来确定具体是哪个外部设备产生的中断
    // 目前简单处理
    
    // 可能的外部中断源：
    // - UART
    // - 键盘
    // - 网络设备
    // - 存储设备等
    
    // TODO: 实现PLIC查询和设备特定的中断处理
}

// 主中断处理函数
void handle_interrupt(uint32_t int_num, struct trap_frame *tf) {
    switch (int_num) {
        case INTERRUPT_S_SOFTWARE:
            DEBUG("Supervisor software interrupt");
            software_interrupt_handler(tf);
            break;
            
        case INTERRUPT_M_SOFTWARE:
            DEBUG("Machine software interrupt");
            software_interrupt_handler(tf);
            break;
            
        case INTERRUPT_S_TIMER:
            DEBUG("Supervisor timer interrupt");
            timer_interrupt_handler(tf);
            break;
            
        case INTERRUPT_M_TIMER:
            timer_interrupt_handler(tf);
            break;
            
        case INTERRUPT_S_EXTERNAL:
            DEBUG("Supervisor external interrupt");
            external_interrupt_handler(tf);
            break;
            
        case INTERRUPT_M_EXTERNAL:
            DEBUG("Machine external interrupt");
            external_interrupt_handler(tf);
            break;
            
        default:
            WARN("Unknown interrupt: %d", int_num);
            break;
    }
}

// 获取定时器tick计数
uint32_t get_timer_ticks(void) {
    return timer_ticks;
}
