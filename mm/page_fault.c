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

// 页面大小定义
#define PAGE_SIZE       4096
#define PAGE_MASK       (PAGE_SIZE - 1)

// 页面权限定义
#define PAGE_READ       0x01
#define PAGE_WRITE      0x02
#define PAGE_EXEC       0x04
#define PAGE_USER       0x08

// 内存布局定义
#define KERNEL_BASE     0x80000000
#define USER_BASE       0x00400000
#define USER_STACK_TOP  0x7FFFF000

// 简单的页面分配状态跟踪
static uint32_t next_free_page = 0x81000000; // 从内核后面开始分配

// 分配一个物理页面 (简单实现)
static uint32_t alloc_page(void) {
    uint32_t page = next_free_page;
    next_free_page += PAGE_SIZE;
    
    // 简单检查是否超出可用内存 (假设有32MB内存)
    if (next_free_page > 0x82000000) {
        ERROR("Out of memory");
        return 0;
    }
    
    DEBUG("Allocated page at 0x%08x", page);
    return page;
}

// 检查地址是否在有效范围内
static int is_valid_address(uint32_t addr) {
    // 检查是否在用户空间范围内
    if (addr >= USER_BASE && addr < USER_STACK_TOP) {
        return 1;
    }
    
    // 检查是否在内核空间范围内
    if (addr >= KERNEL_BASE) {
        return 1;
    }
    
    return 0;
}

// 处理缺页错误 (简单实现)
static int handle_page_fault(uint32_t fault_addr, uint32_t cause) {
    DEBUG("Handling page fault at 0x%08x, cause: %d", fault_addr, cause);
    
    // 检查地址是否有效
    if (!is_valid_address(fault_addr)) {
        ERROR("Invalid address: 0x%08x", fault_addr);
        return 0;
    }
    
    // 对齐到页面边界
    uint32_t page_addr = fault_addr & ~PAGE_MASK;
    
    // 分配新页面
    uint32_t phys_page = alloc_page();
    if (phys_page == 0) {
        ERROR("Failed to allocate page for address 0x%08x", fault_addr);
        return 0;
    }
    
    // TODO: 在真实的实现中，这里需要：
    // 1. 更新页表，建立虚拟地址到物理地址的映射
    // 2. 设置适当的页面权限
    // 3. 刷新TLB
    
    INFO("Mapped virtual page 0x%08x to physical page 0x%08x", page_addr, phys_page);
    return 1; // 成功处理
}

// 页错误处理主函数
void page_fault_handler(uint32_t cause, uint32_t fault_addr, struct trap_frame *tf) {
    const char *cause_str;
    
    switch (cause) {
        case EXCEPTION_INST_PAGE_FAULT:
            cause_str = "Instruction page fault";
            break;
        case EXCEPTION_LOAD_PAGE_FAULT:
            cause_str = "Load page fault";
            break;
        case EXCEPTION_STORE_PAGE_FAULT:
            cause_str = "Store page fault";
            break;
        default:
            cause_str = "Unknown page fault";
            break;
    }
    
    DEBUG("%s at address 0x%08x, PC: 0x%08x", cause_str, fault_addr, tf->sepc);
    
    // 尝试处理页错误
    if (handle_page_fault(fault_addr, cause)) {
        DEBUG("Page fault handled successfully");
        return;
    }
    
    // 无法处理的页错误
    ERROR("Unrecoverable %s at address 0x%08x", cause_str, fault_addr);
    ERROR("Process was executing at PC: 0x%08x", tf->sepc);
    
    // 打印一些调试信息
    ERROR("Register dump:");
    ERROR("  ra = 0x%08x, sp = 0x%08x, gp = 0x%08x", tf->regs[1], tf->regs[2], tf->regs[3]);
    ERROR("  a0 = 0x%08x, a1 = 0x%08x, a2 = 0x%08x", tf->regs[10], tf->regs[11], tf->regs[12]);
    
    PANIC("Unrecoverable page fault at 0x%08x", fault_addr);
}
