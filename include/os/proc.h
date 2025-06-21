#ifndef OS_PROC_H
#define OS_PROC_H

#include "types.h"

// 进程状态枚举
typedef enum {
    PROC_UNUSED = 0,    // 未使用
    PROC_RUNNABLE,      // 就绪态
    PROC_RUNNING,       // 运行态
    PROC_BLOCKED,       // 阻塞态
    PROC_ZOMBIE         // 僵尸态
} proc_state_t;

// 进程上下文 - 保存寄存器状态
typedef struct {
    uint32_t ra;    // 返回地址
    uint32_t sp;    // 栈指针
    uint32_t gp;    // 全局指针
    uint32_t tp;    // 线程指针
    uint32_t t0, t1, t2;    // 临时寄存器
    uint32_t s0, s1;        // 保存寄存器
    uint32_t a0, a1, a2, a3, a4, a5, a6, a7;  // 参数寄存器
    uint32_t s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;  // 保存寄存器
    uint32_t t3, t4, t5, t6;    // 临时寄存器
} context_t;

// 进程控制块 PCB
typedef struct proc {
    uint32_t pid;               // 进程ID
    proc_state_t state;         // 进程状态
    uint32_t priority;          // 优先级
    uint32_t time_slice;        // 时间片
    uint32_t arrival_time;      // 到达时间
    uint32_t burst_time;        // 执行时间
    uint32_t remaining_time;    // 剩余时间
    uint32_t wait_time;         // 等待时间
    uint32_t turnaround_time;   // 周转时间
    
    context_t context;          // 进程上下文
    uint32_t stack_base;        // 栈基址
    uint32_t stack_size;        // 栈大小
    
    struct proc* parent;        // 父进程
    struct proc* next;          // 进程链表指针
    
    char name[16];              // 进程名
} proc_t;

// 进程表
#define MAX_PROC 64
extern proc_t proc_table[MAX_PROC];
extern proc_t* current_proc;

// 进程管理函数
void proc_init(void);
proc_t* proc_create(const char* name, void (*entry)(void), uint32_t priority);
void proc_destroy(proc_t* proc);
void proc_exit(int exit_code);
proc_t* proc_find(uint32_t pid);
void proc_dump(void);

// 进程状态转换
void proc_set_state(proc_t* proc, proc_state_t state);
const char* proc_state_name(proc_state_t state);

#endif // OS_PROC_H
