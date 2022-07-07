set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(triple arm-none-linux-gnueabihf)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_ASM_COMPILER_TARGET ${triple})

set(CMAKE_EXE_LINKER_FLAGS -fuse-ld=lld)
add_compile_options(--verbose)
add_link_options(--verbose)

set(GCC_INSTALL_PREFIX "/usr/local/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf")
set(CMAKE_SYSROOT ${GCC_INSTALL_PREFIX}/${triple}/libc)

set(CMAKE_C_COMPILER_EXTERNAL_TOOLCHAIN ${GCC_INSTALL_PREFIX})
set(CMAKE_CXX_COMPILER_EXTERNAL_TOOLCHAIN ${GCC_INSTALL_PREFIX})