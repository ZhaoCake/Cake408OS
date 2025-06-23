#include "../include/sbi.h"

/* 基础SBI调用函数 - 使用内联汇编实现 */
struct sbi_ret sbi_ecall(int ext, int fid, unsigned long arg0,
                         unsigned long arg1, unsigned long arg2,
                         unsigned long arg3, unsigned long arg4,
                         unsigned long arg5)
{
    register long a0 __asm__("a0") = arg0;
    register long a1 __asm__("a1") = arg1;
    register long a2 __asm__("a2") = arg2;
    register long a3 __asm__("a3") = arg3;
    register long a4 __asm__("a4") = arg4;
    register long a5 __asm__("a5") = arg5;
    register long a6 __asm__("a6") = fid;
    register long a7 __asm__("a7") = ext;

    __asm__ __volatile__("ecall"
                         : "=r"(a0), "=r"(a1)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5),
                           "r"(a6), "r"(a7)
                         : "memory");
    
    return (struct sbi_ret){.error = a0, .value = a1};
}

/* SBI Base扩展调用 */
struct sbi_ret sbi_get_spec_version(void)
{
    return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_SPEC_VERSION, 0, 0, 0, 0, 0, 0);
}

struct sbi_ret sbi_get_impl_id(void)
{
    return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_IMPL_ID, 0, 0, 0, 0, 0, 0);
}

struct sbi_ret sbi_get_impl_version(void)
{
    return sbi_ecall(SBI_EXT_BASE, SBI_BASE_GET_IMPL_VERSION, 0, 0, 0, 0, 0, 0);
}

struct sbi_ret sbi_probe_extension(int extid)
{
    return sbi_ecall(SBI_EXT_BASE, SBI_BASE_PROBE_EXT, extid, 0, 0, 0, 0, 0);
}

/* SBI Timer扩展调用 */
struct sbi_ret sbi_set_timer(uint64_t stime_value)
{
#if __riscv_xlen == 32
    return sbi_ecall(SBI_EXT_TIMER, SBI_TIMER_SET_TIMER, 
                     stime_value & 0xFFFFFFFF, stime_value >> 32, 0, 0, 0, 0);
#else
    return sbi_ecall(SBI_EXT_TIMER, SBI_TIMER_SET_TIMER, 
                     stime_value, 0, 0, 0, 0, 0);
#endif
}

/* SBI System Reset扩展调用 */
struct sbi_ret sbi_system_reset(uint32_t reset_type, uint32_t reset_reason)
{
    return sbi_ecall(SBI_EXT_SRST, SBI_SRST_SYSTEM_RESET, 
                     reset_type, reset_reason, 0, 0, 0, 0);
}

/* SBI Debug Console扩展调用 */
struct sbi_ret sbi_console_write(unsigned long num_bytes, unsigned long base_addr_lo, unsigned long base_addr_hi)
{
    return sbi_ecall(SBI_EXT_DBCN, SBI_DBCN_CONSOLE_WRITE,
                     num_bytes, base_addr_lo, base_addr_hi, 0, 0, 0);
}

struct sbi_ret sbi_console_read(unsigned long num_bytes, unsigned long base_addr_lo, unsigned long base_addr_hi)
{
    return sbi_ecall(SBI_EXT_DBCN, SBI_DBCN_CONSOLE_READ,
                     num_bytes, base_addr_lo, base_addr_hi, 0, 0, 0);
}

struct sbi_ret sbi_console_write_byte(uint8_t byte)
{
    return sbi_ecall(SBI_EXT_DBCN, SBI_DBCN_CONSOLE_WRITE_BYTE,
                     byte, 0, 0, 0, 0, 0);
}

/* 便利函数实现 */

/* 使用新的SBI Debug Console扩展进行控制台输出 */
void sbi_console_putchar(int ch)
{
    struct sbi_ret ret;
    
    /* 首先检查是否支持Debug Console扩展 */
    ret = sbi_probe_extension(SBI_EXT_DBCN);
    if (ret.error == SBI_SUCCESS && ret.value > 0) {
        /* 使用Debug Console Write Byte函数 */
        sbi_console_write_byte((uint8_t)ch);
    } else {
        /* 如果不支持Debug Console，则无法输出 */
        /* 在实际项目中，可以考虑使用UART等其他方式 */
    }
}

/* 使用新的SBI Debug Console扩展进行控制台输入 */
int sbi_console_getchar(void)
{
    struct sbi_ret ret;
    
    /* 首先检查是否支持Debug Console扩展 */
    ret = sbi_probe_extension(SBI_EXT_DBCN);
    if (ret.error == SBI_SUCCESS && ret.value > 0) {
        /* Debug Console扩展不提供单字符读取，返回-1表示不可用 */
        return -1;
    } else {
        /* 如果不支持Debug Console，则无法输入 */
        return -1;
    }
}

/* 系统关机 */
void sbi_shutdown(void)
{
    struct sbi_ret ret;
    
    /* 首先检查是否支持System Reset扩展 */
    ret = sbi_probe_extension(SBI_EXT_SRST);
    if (ret.error == SBI_SUCCESS && ret.value > 0) {
        /* 使用新的System Reset扩展进行关机 */
        sbi_system_reset(SBI_SRST_RESET_TYPE_SHUTDOWN, SBI_SRST_RESET_REASON_NONE);
    }
    
    /* 如果System Reset调用失败或不支持，进入无限循环 */
    while (1) {
        asm volatile("wfi");
    }
}
