#include "../include/os/types.h"
#include "../include/os/kprintf.h"

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
    
    kputs("\nKernel is running...");
    kputs("This is a minimal OS kernel for 408 OS course.");
    kputs("\nNext steps:");
    kputs("1. Add process management");
    kputs("2. Implement scheduling algorithms");
    kputs("3. Add memory management");
    kputs("4. Implement file system");
    
    kputs("\nSystem ready! Press Ctrl+A then X to exit QEMU.");
    
    // 系统进入空闲循环
    while (1) {
        // 简单的空闲循环，等待中断
        asm volatile("wfi"); // Wait For Interrupt
    }
}