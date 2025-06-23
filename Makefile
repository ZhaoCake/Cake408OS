# Copyright (c) 2025 ZhaoCake
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Cake408OS Makefile

# 工具链配置
CROSS_COMPILE = riscv64-none-elf-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

# 编译选项
CFLAGS = -march=rv32ima_zicsr_zifencei -mabi=ilp32 -mcmodel=medany
CFLAGS += -fno-builtin -fno-stack-protector -fno-strict-aliasing
CFLAGS += -Wall -Wextra -g -nostdlib -nostdinc
# Include路径设置 - 使头文件可以直接通过 #include <os/xxx.h> 引用
CFLAGS += -Iinclude -I. -Iarch/riscv32/include

# 链接选项
LDFLAGS = -m elf32lriscv -T arch/riscv32/boot/kernel.ld

# 目录和文件
BUILD_DIR = build
FIRMWARE_DIR = firmware
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_BIN = $(BUILD_DIR)/kernel.bin
OPENSBI_BIN = $(FIRMWARE_DIR)/fw_dynamic.bin

# 源文件 - 添加SBI调用模块和内核库
SRCS = kernel/kernel.c \
       kernel/syscall.c \
       tests/sbi_test.c \
       tests/panic_test.c \
       tests/exception_test.c \
       tests/test_runner.c \
       klib/kprintf.c \
       klib/string.c \
       klib/memory.c \
       arch/riscv32/boot/start.S \
       arch/riscv32/boot/trap_entry.S \
       arch/riscv32/sbi/sbi_call.c \
       arch/riscv32/trap/trap.c \
       arch/riscv32/trap/interrupt.c \
       mm/page_fault.c

# 目标文件
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRCS))) \
       $(patsubst %.S,$(BUILD_DIR)/%.o,$(filter %.S,$(SRCS)))

# 默认目标
all: check-tools $(OPENSBI_BIN) $(KERNEL_BIN)
	@echo "=== Build completed successfully! ==="
	@echo "Run with: make run"

# 检查工具链
check-tools:
	@echo "=== Checking toolchain ==="
	@which $(CC) > /dev/null || (echo "Error: $(CC) not found! Install with: sudo apt install gcc-riscv64-unknown-elf" && exit 1)
	@which qemu-system-riscv32 > /dev/null || (echo "Error: qemu-system-riscv32 not found! Install with: sudo apt install qemu-system-misc" && exit 1)
	@echo "Toolchain OK"

# 下载OpenSBI固件
$(OPENSBI_BIN):
	@echo "=== Downloading OpenSBI firmware ==="
	@mkdir -p $(FIRMWARE_DIR)
	@cd $(FIRMWARE_DIR) && \
	wget -q https://github.com/riscv-software-src/opensbi/releases/download/v1.6/opensbi-1.6-rv-bin.tar.xz && \
	tar -xf opensbi-1.6-rv-bin.tar.xz && \
	cp opensbi-1.6-rv-bin/share/opensbi/ilp32/generic/firmware/fw_dynamic.bin . && \
	cp opensbi-1.6-rv-bin/share/opensbi/ilp32/generic/firmware/fw_dynamic.elf . && \
	rm -rf opensbi-1.6-rv-bin opensbi-1.6-rv-bin.tar.xz
	@echo "OpenSBI firmware ready"

# 创建构建目录
$(BUILD_DIR):
	@mkdir -p $(dir $(OBJS))

# 编译规则
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "CC $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	@echo "AS $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# 链接内核
$(KERNEL_ELF): $(OBJS)
	@echo "LD $@"
	@$(LD) $(LDFLAGS) $(OBJS) -o $@

# 生成二进制
$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "OBJCOPY $@"
	@$(OBJCOPY) -O binary $< $@

# 运行系统
run: all
	@echo "=== Starting Cake408OS ==="
	@echo "Press Ctrl+A then X to exit QEMU"
	@qemu-system-riscv32 -machine virt -m 128M \
		-bios $(OPENSBI_BIN) \
		-kernel $(KERNEL_ELF) \
		-nographic

# 调试模式
debug: all
	@echo "=== Starting Cake408OS in debug mode ==="
	@echo "Connect with: $(CROSS_COMPILE)gdb $(KERNEL_ELF) -ex 'target remote :1234'"
	@qemu-system-riscv32 -machine virt -m 128M \
		-bios $(OPENSBI_BIN) \
		-kernel $(KERNEL_ELF) \
		-nographic -s -S

# 反汇编
disasm: $(KERNEL_ELF)
	@echo "Generating disassembly..."
	@$(OBJDUMP) -d $< > $(BUILD_DIR)/kernel.dis
	@echo "Disassembly saved to $(BUILD_DIR)/kernel.dis"

# 清理
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)

# 完全清理（包括固件）
distclean: clean
	@echo "Cleaning all generated files..."
	@rm -rf $(FIRMWARE_DIR)

# 帮助
help:
	@echo "Cake408OS Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build kernel (default)"
	@echo "  run      - Build and run in QEMU"
	@echo "  debug    - Build and run in debug mode"
	@echo "  disasm   - Generate disassembly"
	@echo "  clean    - Clean build files"
	@echo "  distclean- Clean all files including firmware"
	@echo "  help     - Show this help"

.PHONY: all run debug disasm clean distclean help check-tools