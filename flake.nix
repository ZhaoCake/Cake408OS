{
  description = "Cake408OS Development Environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            pkgsCross.riscv32-embedded.buildPackages.gcc
            pkgsCross.riscv32-embedded.buildPackages.gdb
            qemu
            gnumake
          ];

          shellHook = ''
            export CROSS_COMPILE=riscv32-none-elf-
            export CC=riscv32-none-elf-gcc
            export OBJCOPY=riscv32-none-elf-objcopy
            export OBJDUMP=riscv32-none-elf-objdump
            export GDB=riscv32-none-elf-gdb
          '';
        };
      });
}
