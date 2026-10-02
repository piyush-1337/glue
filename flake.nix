{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/?ref=nixpkgs-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
      llvm = pkgs.llvmPackages_23;
    in
    {
      devShells.${system}.default = pkgs.mkShell.override { stdenv = llvm.libcxxStdenv; } {
        packages = [
          llvm.libcxxClang
          llvm.lld
          llvm.libcxx
          llvm.libcxx.dev

          pkgs.cmake
          pkgs.ninja

          llvm.clang-tools
          pkgs.neocmakelsp
        ];

        hardeningDisable = [ "all" ];

        CC = "clang";
        CXX = "clang++";
        CXXFLAGS = "-std=c++23 -stdlib=libc++";
        LDFLAGS = "-fuse-ld=lld -stdlib=libc++ -lc++abi -Wl,-rpath,${llvm.libcxx}/lib";
        LIBCXX_MODULES_JSON = "${llvm.libcxx}/lib/libc++.modules.json";
        LIBCXX_INCLUDE_DIR = "${llvm.libcxx.dev}/include";
        GLIBC_INCLUDE_DIR = "${pkgs.glibc.dev}/include";
      };
    };
}
