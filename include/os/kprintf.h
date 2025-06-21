#ifndef _OS_KPRINTF_H
#define _OS_KPRINTF_H

#include "types.h"

int kprintf(const char *fmt, ...);
int kputs(const char *str);

#endif