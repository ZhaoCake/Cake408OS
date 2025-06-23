#ifndef _DEBUG_DEBUG_H
#define _DEBUG_DEBUG_H

#include <os/kprintf.h>
#include <utils/config.h>
#include <debug/colors.h>

/* 调试输出宏 */
#if CONFIG_DEBUG
#define DEBUG(fmt, ...)                                                        \
    kprintf(COLOR_DIM COLOR_CYAN "[DEBUG]" COLOR_RESET COLOR_DIM " %s:%d: "   \
           fmt COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(fmt, ...) do { } while (0)
#endif

/* 详细启动信息宏 */
#if CONFIG_VERBOSE_BOOT
#define BOOT_INFO(fmt, ...) kprintf(COLOR_BLUE fmt COLOR_RESET, ##__VA_ARGS__)
#else
#define BOOT_INFO(fmt, ...) do { } while (0)
#endif

#endif /* _DEBUG_DEBUG_H */
