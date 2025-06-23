#include <os/types.h>
#include <os/kprintf.h>

/* 声明SBI测试函数 */
extern void sbi_test(void);

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
    
    /* 测试SBI接口 */
    sbi_test();
    
    kputs("All subsystems initialized successfully!");
    
    // 系统进入空闲循环
    while (1) {
        // 简单的空闲循环，等待中断
        asm volatile("wfi"); // Wait For Interrupt
    }
}