#ifndef _DEBUG_H
#define _DEBUG_H

/*
 * 调试工具总入口
 * 包含所有调试相关的宏和函数定义
 * 注意：内核核心的 panic 功能在 <os/panic.h> 中，这里只包含调试增强功能
 */

#include <debug/colors.h>
#include <debug/panic.h>      /* 调试版本的彩色 panic 宏 */
#include <debug/debug.h>
#include <debug/logging.h>
#include <debug/testing.h>

#endif /* _DEBUG_H */
