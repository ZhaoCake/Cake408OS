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

{
  description = "Cake408OS RISC-V Operating System Development Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            # RISC-V工具链
            pkgsCross.riscv64-embedded.buildPackages.gcc
            pkgsCross.riscv64-embedded.buildPackages.binutils
            
            # QEMU模拟器
            qemu
            
            # 基础工具
            wget
            curl
            gnutar
            gzip
            xz
            
            # 调试工具
            gdb
            
            # 构建工具
            gnumake
            
            # Fish shell
            fish
            
            # 文本编辑和开发工具
            vim
            git
          ];

          shellHook = ''
            # 设置环境变量
            export CROSS_COMPILE=riscv64-none-elf-
            export CC=riscv64-none-elf-gcc
            export LD=riscv64-none-elf-ld
            export OBJCOPY=riscv64-none-elf-objcopy
            export OBJDUMP=riscv64-none-elf-objdump
            export GDB=riscv64-none-elf-gdb
            
            echo "🎂 Welcome to Cake408OS Development Environment!"
            echo ""
            echo "Available tools:"
            echo "  • RISC-V Cross Compiler: riscv64-none-elf-gcc"
            echo "  • QEMU RISC-V Emulator: qemu-system-riscv32"
            echo "  • GNU Make: make"
            echo "  • GDB Debugger: riscv64-none-elf-gdb"
            echo ""
            echo "Quick start:"
            echo "  make all     - Build the kernel"
            echo "  make run     - Build and run in QEMU"
            echo "  make debug   - Build and run in debug mode"
            echo "  make help    - Show all available targets"
            echo ""
            
            # 检查工具链是否可用
            if command -v riscv64-none-elf-gcc >/dev/null 2>&1; then
              echo "✅ RISC-V toolchain is ready"
            else
              echo "❌ RISC-V toolchain not found"
            fi
            
            if command -v qemu-system-riscv32 >/dev/null 2>&1; then
              echo "✅ QEMU RISC-V emulator is ready"
            else
              echo "❌ QEMU RISC-V emulator not found"
            fi
            
            echo ""
            echo "🐟 Type 'fish' to start Fish shell, or continue with bash"
            echo "Environment is ready for Cake408OS development!"
          '';
        };
      });
}
