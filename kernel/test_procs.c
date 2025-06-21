#include "../include/os/kprintf.h"
#include "../include/os/proc.h"

// 测试进程函数
void test_proc1(void) {
    kprintf("Process 1 is running!\n");
    // 模拟一些工作
    for (int i = 0; i < 3; i++) {
        kprintf("  Process 1: iteration %d\n", i);
    }
    proc_exit(0);
}

void test_proc2(void) {
    kprintf("Process 2 is running!\n");
    // 模拟一些工作
    for (int i = 0; i < 2; i++) {
        kprintf("  Process 2: iteration %d\n", i);
    }
    proc_exit(0);
}

void test_proc3(void) {
    kprintf("Process 3 is running!\n");
    kprintf("  Process 3: doing some work...\n");
    proc_exit(0);
}
