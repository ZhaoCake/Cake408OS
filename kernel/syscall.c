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
#include <arch/riscv32/include/trap.h>
#include <debug/debug_all.h>

// 系统调用号定义
#define SYS_EXIT        93
#define SYS_READ        63
#define SYS_WRITE       64
#define SYS_OPENAT      56
#define SYS_CLOSE       57
#define SYS_GETPID      172
#define SYS_KILL        129
#define SYS_FORK        220
#define SYS_EXECVE      221

// 当前进程ID (简单实现)
static uint32_t current_pid = 1;

// 系统调用实现函数

static int32_t sys_exit(int32_t status) {
    INFO("Process %d exiting with status %d", current_pid, status);
    // TODO: 实现进程退出逻辑
    return 0;
}

static int32_t sys_read(int32_t fd, char *buf, size_t count) {
    DEBUG("sys_read: fd=%d, buf=0x%08x, count=%d", fd, (uint32_t)buf, count);
    // TODO: 实现文件读取
    // 目前返回0表示EOF
    return 0;
}

static int32_t sys_write(int32_t fd, const char *buf, size_t count) {
    DEBUG("sys_write: fd=%d, buf=0x%08x, count=%d", fd, (uint32_t)buf, count);
    
    // 简单实现：如果是stdout(1)或stderr(2)，直接输出到控制台
    if (fd == 1 || fd == 2) {
        for (size_t i = 0; i < count; i++) {
            if (buf[i] == '\0') break;
            kputc(buf[i]);
        }
        return count;
    }
    
    // TODO: 实现文件写入
    return -1; // 暂时返回错误
}

static int32_t sys_openat(int32_t dirfd, const char *pathname, int32_t flags) {
    DEBUG("sys_openat: dirfd=%d, pathname=0x%08x, flags=%d", dirfd, (uint32_t)pathname, flags);
    // TODO: 实现文件打开
    return -1; // 暂时返回错误
}

static int32_t sys_close(int32_t fd) {
    DEBUG("sys_close: fd=%d", fd);
    // TODO: 实现文件关闭
    return 0;
}

static int32_t sys_getpid(void) {
    return current_pid;
}

static int32_t sys_kill(int32_t pid, int32_t sig) {
    DEBUG("sys_kill: pid=%d, signal=%d", pid, sig);
    // TODO: 实现信号发送
    return -1; // 暂时返回错误
}

static int32_t sys_fork(void) {
    DEBUG("sys_fork called");
    // TODO: 实现进程复制
    return -1; // 暂时返回错误
}

static int32_t sys_execve(const char *filename, char *const argv[], char *const envp[]) {
    DEBUG("sys_execve: filename=0x%08x", (uint32_t)filename);
    // TODO: 实现程序执行
    (void)argv; // 避免unused参数警告
    (void)envp; // 避免unused参数警告
    return -1; // 暂时返回错误
}

// 系统调用处理函数
void syscall_handler(struct trap_frame *tf) {
    uint32_t syscall_num = tf->regs[17]; // a7寄存器包含系统调用号
    uint32_t arg0 = tf->regs[10];        // a0
    uint32_t arg1 = tf->regs[11];        // a1
    uint32_t arg2 = tf->regs[12];        // a2
    uint32_t arg3 = tf->regs[13];        // a3
    uint32_t arg4 = tf->regs[14];        // a4
    uint32_t arg5 = tf->regs[15];        // a5
    
    int32_t ret = -1; // 默认返回值
    
    // 避免unused参数警告
    (void)arg3;
    (void)arg4;
    (void)arg5;
    
    DEBUG("System call %d called with args: %d, %d, %d", syscall_num, arg0, arg1, arg2);
    
    switch (syscall_num) {
        case SYS_EXIT:
            ret = sys_exit((int32_t)arg0);
            break;
            
        case SYS_READ:
            ret = sys_read((int32_t)arg0, (char *)arg1, (size_t)arg2);
            break;
            
        case SYS_WRITE:
            ret = sys_write((int32_t)arg0, (const char *)arg1, (size_t)arg2);
            break;
            
        case SYS_OPENAT:
            ret = sys_openat((int32_t)arg0, (const char *)arg1, (int32_t)arg2);
            break;
            
        case SYS_CLOSE:
            ret = sys_close((int32_t)arg0);
            break;
            
        case SYS_GETPID:
            ret = sys_getpid();
            break;
            
        case SYS_KILL:
            ret = sys_kill((int32_t)arg0, (int32_t)arg1);
            break;
            
        case SYS_FORK:
            ret = sys_fork();
            break;
            
        case SYS_EXECVE:
            ret = sys_execve((const char *)arg0, (char *const *)arg1, (char *const *)arg2);
            break;
            
        default:
            WARN("Unknown system call: %d", syscall_num);
            ret = -1;
            break;
    }
    
    // 将返回值放入a0寄存器
    tf->regs[10] = (uint32_t)ret;
    
    // 移动到下一条指令
    tf->sepc += 4;
}
