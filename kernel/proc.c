#include "../include/os/proc.h"
#include "../include/os/kprintf.h"

// 进程表和当前进程指针
proc_t proc_table[MAX_PROC];
proc_t* current_proc = NULL;
static uint32_t next_pid = 1;

// 初始化进程管理
void proc_init(void) {
    kprintf("Initializing process management...\n");
    
    // 清空进程表
    for (int i = 0; i < MAX_PROC; i++) {
        proc_table[i].pid = 0;
        proc_table[i].state = PROC_UNUSED;
        proc_table[i].next = NULL;
    }
    
    // 创建空闲进程(PID 0)
    proc_table[0].pid = 0;
    proc_table[0].state = PROC_RUNNING;
    proc_table[0].priority = 0;
    proc_table[0].parent = NULL;
    proc_table[0].next = NULL;
    
    // 简单的字符串复制
    const char* idle_name = "idle";
    char* dest = proc_table[0].name;
    for (int i = 0; i < 15 && idle_name[i]; i++) {
        dest[i] = idle_name[i];
    }
    dest[4] = '\0';
    
    current_proc = &proc_table[0];
    
    kprintf("Process management initialized\n");
    kprintf("- Process table size: %d\n", MAX_PROC);
    kprintf("- Idle process created (PID 0)\n");
}

// 创建新进程
proc_t* proc_create(const char* name, void (*entry)(void), uint32_t priority) {
    // 查找空闲的进程表项
    proc_t* proc = NULL;
    for (int i = 1; i < MAX_PROC; i++) {  // 从1开始，0是空闲进程
        if (proc_table[i].state == PROC_UNUSED) {
            proc = &proc_table[i];
            break;
        }
    }
    
    if (!proc) {
        kprintf("ERROR: No free process slot available\n");
        return NULL;
    }
    
    // 初始化PCB
    proc->pid = next_pid++;
    proc->state = PROC_RUNNABLE;
    proc->priority = priority;
    proc->time_slice = 10;  // 默认时间片
    proc->arrival_time = 0; // 简化，设为0
    proc->burst_time = 0;
    proc->remaining_time = 0;
    proc->wait_time = 0;
    proc->turnaround_time = 0;
    
    // 设置栈空间 (简化实现，实际应该分配真实内存)
    proc->stack_base = 0x81000000 + (proc->pid * 0x10000);  // 每个进程64KB栈
    proc->stack_size = 0x10000;
    
    // 初始化上下文
    for (uint32_t* reg = (uint32_t*)&proc->context; 
         reg < (uint32_t*)(&proc->context + 1); reg++) {
        *reg = 0;
    }
    proc->context.sp = proc->stack_base + proc->stack_size - 4;  // 栈顶
    // 暂时不设置入口地址，避免类型转换问题
    // proc->context.ra = entry;  // 入口地址
    
    proc->parent = current_proc;
    proc->next = NULL;
    
    // 复制进程名
    char* dest = proc->name;
    for (int i = 0; i < 15 && name[i]; i++) {
        dest[i] = name[i];
    }
    dest[15] = '\0';
    
    kprintf("Process created: PID=%d, name=%s, priority=%d\n", 
            proc->pid, proc->name, proc->priority);
    
    return proc;
}

// 销毁进程
void proc_destroy(proc_t* proc) {
    if (!proc || proc->state == PROC_UNUSED) {
        return;
    }
    
    kprintf("Destroying process: PID=%d, name=%s\n", proc->pid, proc->name);
    
    // 设置为未使用状态
    proc->state = PROC_UNUSED;
    proc->pid = 0;
    proc->parent = NULL;
    proc->next = NULL;
    
    // 清空进程名
    for (int i = 0; i < 16; i++) {
        proc->name[i] = '\0';
    }
}

// 进程退出
void proc_exit(int exit_code) {
    if (!current_proc) {
        return;
    }
    
    kprintf("Process %d (%s) exiting with code %d\n", 
            current_proc->pid, current_proc->name, exit_code);
    
    current_proc->state = PROC_ZOMBIE;
    // 在实际调度中，这里会触发调度器切换到下一个进程
}

// 根据PID查找进程
proc_t* proc_find(uint32_t pid) {
    for (int i = 0; i < MAX_PROC; i++) {
        if (proc_table[i].state != PROC_UNUSED && proc_table[i].pid == pid) {
            return &proc_table[i];
        }
    }
    return NULL;
}

// 设置进程状态
void proc_set_state(proc_t* proc, proc_state_t state) {
    if (!proc) return;
    
    proc_state_t old_state = proc->state;
    proc->state = state;
    
    kprintf("Process %d (%s): %s -> %s\n", 
            proc->pid, proc->name, 
            proc_state_name(old_state), 
            proc_state_name(state));
}

// 获取进程状态名称
const char* proc_state_name(proc_state_t state) {
    switch (state) {
        case PROC_UNUSED:   return "UNUSED";
        case PROC_RUNNABLE: return "RUNNABLE";
        case PROC_RUNNING:  return "RUNNING";
        case PROC_BLOCKED:  return "BLOCKED";
        case PROC_ZOMBIE:   return "ZOMBIE";
        default:            return "UNKNOWN";
    }
}

// 打印进程信息
void proc_dump(void) {
    kprintf("\n=== Process Table ===\n");
    kprintf("PID  Name            State      Priority  Parent\n");
    kprintf("---  ----            -----      --------  ------\n");
    
    for (int i = 0; i < MAX_PROC; i++) {
        if (proc_table[i].state != PROC_UNUSED) {
            proc_t* proc = &proc_table[i];
            uint32_t parent_pid = proc->parent ? proc->parent->pid : 0;
            
            kprintf("%-4d %-15s %-10s %-8d  %-6d\n",
                    proc->pid, proc->name, proc_state_name(proc->state),
                    proc->priority, parent_pid);
        }
    }
    kprintf("=====================\n\n");
}
