#ifndef _OS_KPRINTF_H
#define _OS_KPRINTF_H

#include <os/types.h>
#include <os/stdarg.h>

int kprintf(const char *fmt, ...);
int kvprintf(const char *fmt, va_list args);
int kputs(const char *str);

#endif