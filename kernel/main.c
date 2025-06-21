#include "../include/os/types.h"
#include "../include/os/kprintf.h"
#include "../include/os/proc.h"

// 声明测试进程函数
void test_proc1(void);
void test_proc2(void);
void test_proc3(void);

void kernel_main(void) {
    // 内核启动信息
    kputs("========================================");
    kputs("    Cake408OS - RISC-V Teaching OS    ");
    kputs("========================================");
    kprintf("Kernel started successfully!\n");
    
    kputs("System Information:");
    kprintf("- Architecture: RISC-V 32-bit\n");
    kprintf("- Platform: QEMU virt machine\n");
    kprintf("- Firmware: OpenSBI\n");
    kprintf("- Kernel loaded at: 0x80200000\n");
    kprintf("- Stack pointer: 0x80300000\n");
    
    kputs("\nInitializing subsystems...");
    
    // 初始化进程管理
    proc_init();
    
    // 创建测试进程
    kputs("\nCreating test processes...");
    proc_t* p1 = proc_create("test1", test_proc1, 1);
    proc_t* p2 = proc_create("test2", test_proc2, 2);
    proc_t* p3 = proc_create("test3", test_proc3, 1);
    
    // 显示进程表
    proc_dump();
    
    // 模拟进程状态变化
    kputs("Simulating process state changes...");
    if (p1) proc_set_state(p1, PROC_RUNNING);
    if (p1) proc_set_state(p1, PROC_BLOCKED);
    if (p2) proc_set_state(p2, PROC_RUNNING);
    if (p1) proc_set_state(p1, PROC_RUNNABLE);
    
    // 再次显示进程表
    proc_dump();
    
    kputs("Process management demonstration completed!");
    kputs("Note: Actual process scheduling not implemented yet.");
    
    kputs("\nSystem ready! Press Ctrl+A then X to exit QEMU.");
    
    // 系统进入空闲循环
    while (1) {
        // 简单的空闲循环，等待中断
        asm volatile("wfi"); // Wait For Interrupt
    }
}