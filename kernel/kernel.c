#include <os/types.h>
#include <os/kprintf.h>
#include <os/kernel.h>
#include <debug/debug_all.h>

#if CONFIG_RUN_TESTS
#include "../tests/tests.h"
#endif

void kernel_main(void) {
    // 内核启动信息
    kprintf(COLOR_BOLD COLOR_BRIGHT_CYAN);
    kputs("========================================");
    kputs("    Cake408OS - RISC-V Teaching OS    ");
    kputs("========================================");
    kprintf(COLOR_RESET);
    SUCCESS("Kernel started successfully!");
    
#if CONFIG_VERBOSE_BOOT
    INFO("System Information:");
    BOOT_INFO("- Architecture: RISC-V 32-bit\n");
    BOOT_INFO("- Platform: QEMU virt machine\n");
    BOOT_INFO("- Firmware: OpenSBI\n");
    BOOT_INFO("- Kernel loaded at: 0x80200000\n");
    BOOT_INFO("- Stack pointer: 0x80300000\n");
    
    DEBUG("Debug mode enabled");
#endif
    
    INFO("Initializing subsystems...");
    
#if CONFIG_RUN_TESTS
    /* 运行系统测试 */
    DEBUG("Running system tests...");
    run_all_tests();
#else
    DEBUG("System tests disabled");
#endif
    
    SUCCESS("All subsystems initialized successfully!");
    
    // 系统进入空闲循环
    while (1) {
        // 简单的空闲循环，等待中断
        asm volatile("wfi"); // Wait For Interrupt
    }
}