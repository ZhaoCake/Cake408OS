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

#ifndef _ARCH_RISCV32_SBI_H
#define _ARCH_RISCV32_SBI_H

#include <os/types.h>

/* SBI函数ID定义 */
#define SBI_EXT_BASE                 0x10
#define SBI_EXT_TIMER                0x54494D45
#define SBI_EXT_IPI                  0x735049
#define SBI_EXT_RFENCE               0x52464E43
#define SBI_EXT_HSM                  0x48534D
#define SBI_EXT_SRST                 0x53525354
#define SBI_EXT_DBCN                 0x4442434E

/* SBI Base扩展函数 */
#define SBI_BASE_GET_SPEC_VERSION    0x0
#define SBI_BASE_GET_IMPL_ID         0x1
#define SBI_BASE_GET_IMPL_VERSION    0x2
#define SBI_BASE_PROBE_EXT           0x3
#define SBI_BASE_GET_MVENDORID       0x4
#define SBI_BASE_GET_MARCHID         0x5
#define SBI_BASE_GET_MIMPID          0x6

/* SBI Timer扩展函数 */
#define SBI_TIMER_SET_TIMER          0x0

/* SBI System Reset扩展函数 */
#define SBI_SRST_SYSTEM_RESET        0x0

/* SBI Debug Console扩展函数 */
#define SBI_DBCN_CONSOLE_WRITE       0x0
#define SBI_DBCN_CONSOLE_READ        0x1
#define SBI_DBCN_CONSOLE_WRITE_BYTE  0x2

/* SBI Reset类型定义 */
#define SBI_SRST_RESET_TYPE_SHUTDOWN    0x0
#define SBI_SRST_RESET_TYPE_COLD_REBOOT 0x1
#define SBI_SRST_RESET_TYPE_WARM_REBOOT 0x2

/* SBI Reset原因定义 */
#define SBI_SRST_RESET_REASON_NONE      0x0
#define SBI_SRST_RESET_REASON_SYSFAIL   0x1

/* SBI返回值 */
#define SBI_SUCCESS                  0
#define SBI_ERR_FAILED              -1
#define SBI_ERR_NOT_SUPPORTED       -2
#define SBI_ERR_INVALID_PARAM       -3
#define SBI_ERR_DENIED              -4
#define SBI_ERR_INVALID_ADDRESS     -5
#define SBI_ERR_ALREADY_AVAILABLE   -6

/* SBI调用返回结构 */
struct sbi_ret {
    long error;
    long value;
};

/* SBI调用函数声明 */
struct sbi_ret sbi_ecall(int ext, int fid, unsigned long arg0,
                         unsigned long arg1, unsigned long arg2,
                         unsigned long arg3, unsigned long arg4,
                         unsigned long arg5);

/* 高级SBI调用接口 */
struct sbi_ret sbi_get_spec_version(void);
struct sbi_ret sbi_get_impl_id(void);
struct sbi_ret sbi_get_impl_version(void);
struct sbi_ret sbi_probe_extension(int extid);
struct sbi_ret sbi_set_timer(uint64_t stime_value);
struct sbi_ret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason);

/* Debug Console扩展接口 */
struct sbi_ret sbi_console_write(unsigned long num_bytes, unsigned long base_addr_lo, unsigned long base_addr_hi);
struct sbi_ret sbi_console_read(unsigned long num_bytes, unsigned long base_addr_lo, unsigned long base_addr_hi);
struct sbi_ret sbi_console_write_byte(uint8_t byte);

/* 便利函数 */
void sbi_console_putchar(int ch);
int sbi_console_getchar(void);
void sbi_shutdown(void);

#endif /* _ARCH_RISCV32_SBI_H */
