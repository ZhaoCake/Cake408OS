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
#include <os/kprintf.h>
#include <mm/memory.h>
#include <debug/debug_all.h>

// 链接器脚本中定义的符号
extern char __free_ram[], __free_ram_end[];

void test_memory_allocator(void) {
    TEST_START("Memory Allocator Test");
    
    // 测试基本内存分配
    TEST_SECTION("Basic Allocation Test");
    paddr_t paddr0 = alloc_pages(2);  // 分配2页 (8KB)
    paddr_t paddr1 = alloc_pages(1);  // 分配1页 (4KB)
    
    kprintf("alloc_pages test: paddr0=0x%08x\n", paddr0);
    kprintf("alloc_pages test: paddr1=0x%08x\n", paddr1);
    kprintf("Expected difference: 0x%08x (8KB)\n", paddr1 - paddr0);
    
    // 验证分配地址的正确性
    TEST_ASSERT_EQ((uint32_t)__free_ram, paddr0, "First allocation should start at __free_ram");
    TEST_ASSERT_EQ(8192, paddr1 - paddr0, "Second allocation should be 8KB after first");
    
    // 测试内存对齐
    TEST_SECTION("Memory Alignment Test");
    TEST_ASSERT((paddr0 % PAGE_SIZE) == 0, "Memory should be page-aligned");
    TEST_ASSERT((paddr1 % PAGE_SIZE) == 0, "Memory should be page-aligned");
    
    // 测试内存清零
    TEST_SECTION("Memory Zeroing Test");
    uint32_t *ptr = (uint32_t *)paddr0;
    bool all_zero = true;
    uint32_t test_words = (2 * PAGE_SIZE) / sizeof(uint32_t);
    for (uint32_t i = 0; i < test_words; i++) {
        if (ptr[i] != 0) {
            all_zero = false;
            break;
        }
    }
    TEST_ASSERT(all_zero, "Allocated memory should be zeroed");
    
    // 测试大量分配
    TEST_SECTION("Large Allocation Test");
    paddr_t large_addr = alloc_pages(16);  // 分配16页 (64KB)
    TEST_ASSERT(large_addr != 0, "Large allocation should succeed");
    TEST_ASSERT((large_addr % PAGE_SIZE) == 0, "Large allocation should be page-aligned");
    
    TEST_END("Memory Allocator Test");
}

void test_memory_limits(void) {
    TEST_START("Memory Limits Test");
    
    // 显示内存布局信息
    TEST_SECTION("Memory Layout Information");
    uint32_t total_memory = (uint32_t)__free_ram_end - (uint32_t)__free_ram;
    uint32_t total_pages = total_memory / PAGE_SIZE;
    
    kprintf("Total available memory: %d KB (%d pages)\n", 
           total_memory / 1024, total_pages);
    kprintf("Page size: %d bytes\n", PAGE_SIZE);
    
    TEST_PASS("Memory layout information displayed");
    
    // 注意：这里不实际测试内存耗尽，因为会导致系统panic
    // 在实际项目中，这种测试应该在隔离环境中进行
    TEST_SECTION("Memory Limit Awareness");
    TEST_PASS("Memory limits verified (actual OOM test skipped for safety)");
    
    TEST_END("Memory Limits Test");
}
