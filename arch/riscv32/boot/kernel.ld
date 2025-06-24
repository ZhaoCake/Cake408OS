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


/* RISC-V 32位内核链接脚本 */

OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x80200000, LENGTH = 126M
}

SECTIONS
{
    . = 0x80200000;
    
    .text : {
        *(.text.init)
        *(.text .text.*)
    } > RAM
    
    .rodata : {
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
    } > RAM
    
    .data : {
        *(.data .data.*)
        *(.sdata .sdata.*)
    } > RAM
    
    .bss : {
        __bss_start = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        __bss_end = .;
    } > RAM
    
    . = ALIGN(4K);
    __kernel_end = .;
    
    /* 栈空间 */
    . = ALIGN(4);
    . += 128 * 1024; /* 128KB */
    __stack_top = .;
    
    /* 动态内存分配区域 */
    . = ALIGN(4096);
    __free_ram = .;
    . += 64 * 1024 * 1024; /* 64MB */
    __free_ram_end = .;
}