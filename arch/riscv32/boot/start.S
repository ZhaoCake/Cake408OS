# RISC-V 32位内核启动代码
.section .text.init
.global _start

_start:
    # 设置栈指针 (在内核地址空间上方)
    li sp, 0x80300000
    
    # 清空BSS段
    la t0, __bss_start
    la t1, __bss_end
clear_bss:
    beq t0, t1, bss_cleared
    sw zero, 0(t0)
    addi t0, t0, 4
    j clear_bss
    
bss_cleared:
    # 跳转到C语言main函数
    call kernel_main
    
    # 如果main返回，进入无限循环
halt:
    wfi
    j halt