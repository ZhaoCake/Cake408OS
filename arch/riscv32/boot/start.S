# Copyright (c) 2025 ZhaoCake
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 

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