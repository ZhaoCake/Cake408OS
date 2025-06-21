# 1 最小内核实现

现在来对最小内核实现这一部分进行代码回读与解释。

## 1.1 boot

首先可以看Boot部分，如果不是写裸机，可能对于ld和start.S不是很熟悉，做嵌入式的可能见过不少，但是不一定去看过里面是怎么回事。进行一些解释。

### kernel.ld

链接脚本负责告诉链接器如何组织内核的内存布局。

```ld
OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv", "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)
```
- `OUTPUT_FORMAT`：指定输出为32位小端RISC-V ELF格式
- `OUTPUT_ARCH`：目标架构为RISC-V
- `ENTRY`：程序入口点为`_start`符号

```ld
MEMORY
{
    RAM (rwx) : ORIGIN = 0x80200000, LENGTH = 126M
}
```
内存布局定义：
- 起始地址：`0x80200000`（避开OpenSBI的`0x80000000-0x80200000`区域）
- 长度：126MB
- 权限：读写执行

```ld
SECTIONS
{
    . = 0x80200000;
    
    .text : {
        *(.text.init)    # 启动代码放在最前面
        *(.text .text.*)
    } > RAM
```
段布局：
- `.text`：代码段，从`0x80200000`开始
- `.text.init`：启动代码优先放置
- 其他代码段按顺序排列

```ld
    .bss : {
        __bss_start = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        __bss_end = .;
    } > RAM
```
BSS段（未初始化数据）：
- `__bss_start`和`__bss_end`：提供给启动代码清零BSS段

### start.S

启动汇编代码是内核的第一段执行代码，负责基础的硬件初始化。

```assembly
.section .text.init
.global _start

_start:
```
- `.section .text.init`：将代码放入`.text.init`段（链接脚本中优先放置）
- `.global _start`：导出`_start`符号，作为程序入口点

#### 栈指针设置
```assembly
li sp, 0x80300000
```
设置栈指针：
- `li`（load immediate）：加载立即数指令
- `sp`：栈指针寄存器
- `0x80300000`：栈顶地址，在内核代码区域（`0x80200000`）上方1MB处

#### BSS段清零
```assembly
la t0, __bss_start
la t1, __bss_end
clear_bss:
    beq t0, t1, bss_cleared
    sw zero, 0(t0)
    addi t0, t0, 4
    j clear_bss
```
C语言要求未初始化全局变量为0，需要手动清零BSS段：
- `la`（load address）：加载地址到寄存器
- `__bss_start`、`__bss_end`：由链接脚本定义的BSS段边界
- `beq`：相等时跳转（branch if equal）
- `sw zero, 0(t0)`：将0写入t0指向的内存
- `addi t0, t0, 4`：地址递增4字节（32位系统）

#### 跳转到C代码
```assembly
bss_cleared:
    call kernel_main
```
汇编初始化完成后，跳转到C语言的`kernel_main`函数：
- `call`：函数调用指令，会保存返回地址

#### 系统停机
```assembly
halt:
    wfi
    j halt
```
如果`kernel_main`返回（正常情况下不应该），系统进入停机状态：
- `wfi`（wait for interrupt）：等待中断，节省功耗
- `j halt`：无条件跳转，形成无限循环

## 1.2 内存布局

经过上述设置，内核的内存布局为：
```
0x80000000 - 0x80200000: OpenSBI固件区域 (2MB)
0x80200000 - 0x80300000: 内核代码和数据 (1MB)
0x80300000 - 0x88000000: 内核栈空间 (125MB)
```

这种布局确保了：
1. 与OpenSBI固件不冲突
2. 内核有足够的代码空间
3. 栈有充足的增长空间

## 1.3 UART驱动实现

UART是系统启动后第一个需要工作的设备，负责向外输出调试信息。

### UART硬件接口
```c
// UART 寄存器地址 (QEMU virt machine)
#define UART_BASE 0x10000000
#define UART_REG_TXFIFO   0x00
#define UART_REG_LSR      0x05

static volatile uint8_t *uart_base = (uint8_t*)UART_BASE;
```

QEMU virt平台的UART控制器位于`0x10000000`：
- `UART_REG_TXFIFO (0x00)`：发送数据FIFO
- `UART_REG_LSR (0x05)`：线路状态寄存器
- `volatile`关键字：防止编译器优化内存访问

### 字符输出函数
```c
void uart_putchar(char c) {
    // 等待发送FIFO有空间
    while ((uart_base[UART_REG_LSR] & 0x20) == 0);
    uart_base[UART_REG_TXFIFO] = c;
}
```

发送单个字符的流程：
1. 检查LSR寄存器的bit 5（THR空标志）
2. 等待发送缓冲区空闲（`0x20`位为1）
3. 将字符写入TXFIFO寄存器

### 字符串输出函数
```c
void uart_puts(const char *str) {
    while (*str) {
        if (*str == '\n') {
            uart_putchar('\r');
        }
        uart_putchar(*str++);
    }
}
```

处理换行符：
- 遇到`\n`时先发送`\r`（回车符）
- 确保终端正确显示换行

## 1.4 内核printf实现

为了调试方便，实现了简化版的printf函数。

### 数字转换函数
```c
static void print_number(uint32_t num, int base) {
    char buf[32];
    int i = 0;
    
    if (num == 0) {
        uart_putchar('0');
        return;
    }
    
    while (num > 0) {
        int digit = num % base;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        num /= base;
    }
    
    // 反向输出
    while (i > 0) {
        uart_putchar(buf[--i]);
    }
}
```

支持十进制和十六进制输出：
- 除法取余获取各位数字
- 0-9映射到'0'-'9'，10-15映射到'a'-'f'
- 由于是从低位到高位计算，需要反向输出

### 格式化输出函数
```c
int kprintf(const char *fmt, ...) {
    const char *p = fmt;
    uint32_t *args = (uint32_t*)((char*)&fmt + sizeof(fmt));
    int arg_index = 0;
    
    while (*p) {
        if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
                case 'd':
                    print_number(args[arg_index++], 10);
                    break;
                case 'x':
                    print_number(args[arg_index++], 16);
                    break;
                case 's':
                    uart_puts("(string)"); // 简化实现
                    arg_index++;
                    break;
                case 'c':
                    uart_putchar((char)args[arg_index++]);
                    break;
                default:
                    uart_putchar(*p);
                    break;
            }
        } else {
            uart_putchar(*p);
        }
        p++;
    }
    return 0;
}
```

简化版printf实现：
- 手动解析可变参数（不使用va_list）
- 支持`%d`（十进制）、`%x`（十六进制）、`%c`（字符）格式
- `%s`格式暂时简化为固定字符串

## 1.5 内核主函数

内核的C语言入口点，负责系统初始化和主循环。

```c
void kernel_main(void) {
    // 内核启动信息
    kputs("========================================");
    kputs("    Cake408OS - RISC-V Teaching OS    ");
    kputs("========================================");
    kprintf("Kernel started successfully!\n");
    
    kputs("System Information:");
    kprintf("- Architecture: RISC-V 32-bit\n");
    kprintf("- Platform: QEMU virt machine\n");
    kprintf("- Firmware: OpenSBI\n");
    kprintf("- Kernel loaded at: 0x80200000\n");
    kprintf("- Stack pointer: 0x80300000\n");  // 在start.S中被设置
    
    kputs("\nKernel is running...");
    kputs("This is a minimal OS kernel for 408 OS course.");
    kputs("\nNext steps:");
    kputs("1. Add process management");
    kputs("2. Implement scheduling algorithms");
    kputs("3. Add memory management");
    kputs("4. Implement file system");
    
    kputs("\nSystem ready! Press Ctrl+A then X to exit QEMU.");
    
    // 系统进入空闲循环
    while (1) {
        asm volatile("wfi"); // Wait For Interrupt
    }
}
```

主函数功能：
1. 输出启动信息和系统状态
2. 显示内存布局信息
3. 说明后续开发计划
4. 进入无限循环等待中断

`wfi`指令的作用：
- 让CPU进入低功耗状态
- 等待中断唤醒
- 在QEMU中可以降低主机CPU占用率

## 1.6 启动流程总结

完整的系统启动流程：

```
1. QEMU启动 -> 加载OpenSBI固件到0x80000000
2. OpenSBI初始化 -> 跳转到内核_start (0x80200000)
3. start.S执行:
   - 设置栈指针到0x80300000
   - 清零BSS段
   - 调用kernel_main
4. kernel_main执行:
   - 初始化UART输出
   - 显示系统信息
   - 进入主循环
```

这个最小内核虽然功能简单，但已经建立了完整的启动框架，为后续添加调度、内存管理等功能打下了基础。

编译后结果如下：

```shell
❰zhaocake❙~/WorkSpace/RISCV/Cake408OS(git≠✱main)❱✔≻ tree build/
build/
├── arch
│   └── riscv32
│       └── boot
│           └── start.o
├── drivers
│   └── uart.o
├── kernel
│   └── main.o
├── kernel.bin
├── kernel.elf
└── lib
    └── kprintf.o

7 directories, 6 files
```