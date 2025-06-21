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
}