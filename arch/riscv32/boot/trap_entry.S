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

.section .text.trap
.globl trap_entry
.align 4

# 异常/中断入口点
trap_entry:
    # 分配栈空间保存寄存器上下文 (36个寄存器 * 4字节)
    addi sp, sp, -(36*4)
    
    # 保存所有通用寄存器 (x1-x31，x0始终为0不需要保存)
    sw x1,  1*4(sp)   # ra
    sw x2,  2*4(sp)   # sp (will be updated)
    sw x3,  3*4(sp)   # gp
    sw x4,  4*4(sp)   # tp
    sw x5,  5*4(sp)   # t0
    sw x6,  6*4(sp)   # t1
    sw x7,  7*4(sp)   # t2
    sw x8,  8*4(sp)   # s0/fp
    sw x9,  9*4(sp)   # s1
    sw x10, 10*4(sp)  # a0
    sw x11, 11*4(sp)  # a1
    sw x12, 12*4(sp)  # a2
    sw x13, 13*4(sp)  # a3
    sw x14, 14*4(sp)  # a4
    sw x15, 15*4(sp)  # a5
    sw x16, 16*4(sp)  # a6
    sw x17, 17*4(sp)  # a7
    sw x18, 18*4(sp)  # s2
    sw x19, 19*4(sp)  # s3
    sw x20, 20*4(sp)  # s4
    sw x21, 21*4(sp)  # s5
    sw x22, 22*4(sp)  # s6
    sw x23, 23*4(sp)  # s7
    sw x24, 24*4(sp)  # s8
    sw x25, 25*4(sp)  # s9
    sw x26, 26*4(sp)  # s10
    sw x27, 27*4(sp)  # s11
    sw x28, 28*4(sp)  # t3
    sw x29, 29*4(sp)  # t4
    sw x30, 30*4(sp)  # t5
    sw x31, 31*4(sp)  # t6
    
    # 保存异常相关的CSR寄存器 (使用S-mode CSR)
    csrr t0, sepc
    sw t0, 32*4(sp)   # sepc
    
    csrr t0, sstatus
    sw t0, 33*4(sp)   # sstatus
    
    csrr t0, scause
    sw t0, 34*4(sp)   # scause
    
    csrr t0, stval
    sw t0, 35*4(sp)   # stval
    
    # 修正保存的sp值（异常前的sp）
    addi t0, sp, 36*4
    sw t0, 2*4(sp)
    
    # 设置参数并调用C语言处理函数
    mv a0, sp          # 传递trap_frame指针作为参数
    call handle_trap
    
    # 恢复CSR寄存器 (使用S-mode CSR)
    lw t0, 32*4(sp)
    csrw sepc, t0
    
    lw t0, 33*4(sp)
    csrw sstatus, t0
    
    # 恢复通用寄存器
    lw x1,  1*4(sp)
    lw x3,  3*4(sp)
    lw x4,  4*4(sp)
    lw x5,  5*4(sp)
    lw x6,  6*4(sp)
    lw x7,  7*4(sp)
    lw x8,  8*4(sp)
    lw x9,  9*4(sp)
    lw x10, 10*4(sp)
    lw x11, 11*4(sp)
    lw x12, 12*4(sp)
    lw x13, 13*4(sp)
    lw x14, 14*4(sp)
    lw x15, 15*4(sp)
    lw x16, 16*4(sp)
    lw x17, 17*4(sp)
    lw x18, 18*4(sp)
    lw x19, 19*4(sp)
    lw x20, 20*4(sp)
    lw x21, 21*4(sp)
    lw x22, 22*4(sp)
    lw x23, 23*4(sp)
    lw x24, 24*4(sp)
    lw x25, 25*4(sp)
    lw x26, 26*4(sp)
    lw x27, 27*4(sp)
    lw x28, 28*4(sp)
    lw x29, 29*4(sp)
    lw x30, 30*4(sp)
    lw x31, 31*4(sp)
    
    # 恢复sp并从异常返回
    lw x2, 2*4(sp)
    sret
