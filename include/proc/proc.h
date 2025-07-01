#ifndef _PROC_H
#define _PROC_H

#include <types.h>

struct proc {
    pid_t pid;
    state_t state;
    context_t context;
    // other process-related information
};

#endif //_PROC_H