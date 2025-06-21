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
            
            # 文本编辑和开发工具
            vim
            git
          ];

          shellHook = ''
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
            echo "You can now run 'make all' to build your OS!"
          '';

          # 设置环境变量
          CROSS_COMPILE = "riscv64-none-elf-";
          CC = "riscv64-none-elf-gcc";
          LD = "riscv64-none-elf-ld";
          OBJCOPY = "riscv64-none-elf-objcopy";
          OBJDUMP = "riscv64-none-elf-objdump";
          GDB = "riscv64-none-elf-gdb";
        };
      });
}
