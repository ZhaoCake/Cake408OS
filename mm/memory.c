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
#include <os/string.h>
#include <os/kernel.h>
#include <mm/memory.h>
#include <debug/debug_all.h>

// 链接器脚本中定义的符号
extern char __free_ram[], __free_ram_end[];

// 世界上最简单的内存分配算法：线性分配器（bump allocator）
paddr_t alloc_pages(uint32_t n) {
    static paddr_t next_paddr = (paddr_t) __free_ram;
    paddr_t paddr = next_paddr;
    next_paddr += n * PAGE_SIZE;

    if (next_paddr > (paddr_t) __free_ram_end) {
        PANIC("out of memory: requested %d pages, but no space left", n);
    }

    // 清零分配的内存，避免未初始化内存导致的问题
    memset((void *) paddr, 0, n * PAGE_SIZE);
    
    DEBUG("Allocated %d pages at 0x%08x", n, paddr);
    return paddr;
}

// 内存管理初始化
void memory_init(void) {
    INFO("Initializing memory management...");
    
    INFO("Memory layout:");
    INFO("  Free RAM start: 0x%08x", (uint32_t)__free_ram);
    INFO("  Free RAM end:   0x%08x", (uint32_t)__free_ram_end);
    INFO("  Available memory: %d KB", 
         ((uint32_t)__free_ram_end - (uint32_t)__free_ram) / 1024);
    
    SUCCESS("Memory management initialized");
}
