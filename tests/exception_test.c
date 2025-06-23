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
#include <debug/debug_all.h>
#include <arch/riscv32/include/trap.h>

// 测试异常处理的功能

// 测试1：非法指令异常
void test_illegal_instruction(void) {
    INFO("Testing illegal instruction exception...");
    
    // 执行一个非法指令 - 这会触发非法指令异常
    // 注意：这个测试会导致系统panic，所以应该最后执行
    asm volatile(".word 0x00000000"); // 非法指令
    
    // 如果程序执行到这里，说明异常处理有问题
    ERROR("ERROR: Illegal instruction was not caught!");
}

// 测试2：断点异常（相对安全的测试）
void test_breakpoint_exception(void) {
    INFO("Testing breakpoint exception...");
    
    // 执行ebreak指令触发断点异常
    asm volatile("ebreak");
    
    SUCCESS("Breakpoint exception handled successfully");
}

// 测试3：系统调用异常
void test_syscall_exception(void) {
    INFO("Testing system call exception...");
    
    uint32_t result;
    
    // 调用sys_getpid系统调用
    asm volatile(
        "li a7, 172\n"    // SYS_GETPID = 172
        "ecall\n"
        "mv %0, a0\n"
        : "=r"(result)
        :
        : "a0", "a7"
    );
    
    INFO("System call returned: %d", result);
    SUCCESS("System call exception handled successfully");
}

// 测试4：内存访问异常（访问无效地址）
void test_memory_access_exception(void) {
    INFO("Testing memory access exception...");
    
    // 尝试访问一个无效的内存地址
    volatile uint32_t *invalid_ptr = (uint32_t *)0x00000000;
    
    // 这应该触发访问故障异常
    uint32_t value = *invalid_ptr;
    
    // 如果程序执行到这里，说明异常处理有问题
    ERROR("ERROR: Memory access exception was not caught! Value: %d", value);
}

// 运行所有安全的异常测试
void run_exception_tests(void) {
    INFO("Starting exception handling tests...");
    
    // 测试断点异常（安全）
    test_breakpoint_exception();
    
    // 测试系统调用异常（安全）
    test_syscall_exception();
    
    SUCCESS("Safe exception tests completed");
    
    // 危险测试，可能会导致系统崩溃
    WARN("About to run dangerous exception tests...");
    WARN("The following tests may cause system panic:");
    
    // 给用户一些时间看到警告信息
    for (volatile int i = 0; i < 1000000; i++);
    
    // 测试内存访问异常（可能导致panic）
    test_memory_access_exception();
    
    // 测试非法指令异常（肯定导致panic）
    test_illegal_instruction();
    
    // 如果执行到这里，说明异常处理有严重问题
    ERROR("CRITICAL: All dangerous exception tests passed - exception handling may be broken!");
}
