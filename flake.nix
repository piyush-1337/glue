{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/?ref=nixpkgs-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      devShells.${system}.default = pkgs.mkShellNoCC {
        packages = with pkgs; [
          gcc16
          cmake
          ninja

          neocmakelsp
          llvmPackages_23.clang-tools
        ];
      };
    };
}
