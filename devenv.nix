{ pkgs, ... }:

{
  # dotenv.enable = true;
  difftastic.enable = true;
  stdenv = pkgs.llvmPackages_20.stdenv;
  
  # https://devenv.sh/packages/
  packages = with pkgs; [
    just
    llvmPackages_20.clang-tools
    lld_20
    libxml2
    libedit
    ocaml
    zlib
    ninja
    cmake
    codespell
    conan
    cppcheck
    ccache
    doxygen
    gtest
    gcovr
    gcc
  ];
  
  env.CC = "${pkgs.llvmPackages_20.clang}/bin/clang";
  env.CXX = "${pkgs.llvmPackages_20.clang}/bin/clang++";
  env.LD_LIBRARY_PATH="${pkgs.llvmPackages_20.libcxx}/lib:${pkgs.zlib}/lib:${pkgs.stdenv.cc.cc.lib}/lib";
  
  # https://devenv.sh/languages/
  # languages.cpp.enable = true;

  # https://devenv.sh/processes/
  # processes.cargo-watch.exec = "cargo-watch";

  # https://devenv.sh/services/
  # services.postgres.enable = true;

  # https://devenv.sh/scripts/

  # enterShell = ''
  #   export-library-path 
  # '';

  # https://devenv.sh/tasks/
  # tasks = {
  #   "myproj:setup".exec = "mytool build";
  #   "devenv:enterShell".after = [ "myproj:setup" ];
  # };

  # https://devenv.sh/tests/
  # enterTest = ''
  #   echo "Running tests"
  #   git --version | grep --color=auto "${pkgs.git.version}"
  # '';

  # https://devenv.sh/git-hooks/
  # git-hooks.hooks.shellcheck.enable = true;

  # See full reference at https://devenv.sh/reference/options/
}
