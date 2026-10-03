cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")

if(DEFINED ENV{LIBCXX_MODULES_JSON} AND NOT DEFINED
   CMAKE_CXX_STDLIB_MODULES_JSON)
  set(CMAKE_CXX_STDLIB_MODULES_JSON "$ENV{LIBCXX_MODULES_JSON}")
endif()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_LIBRARY "libc++")
set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(glue CXX)

if(DEFINED ENV{LIBCXX_INCLUDE_DIR})
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -isystem $ENV{LIBCXX_INCLUDE_DIR}/c++/v1")
endif()

if(DEFINED ENV{GLIBC_INCLUDE_DIR})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -idirafter $ENV{GLIBC_INCLUDE_DIR}")
endif()

include(FetchContent)
set(CLI11_MODULES ON CACHE BOOL "Enable C++20 module support for CLI11" FORCE)
set(CLI11_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLI11_BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    cli11
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.7.2
)
 
FetchContent_MakeAvailable(cli11)

add_executable(glue src/main.cpp)
target_link_libraries(glue PRIVATE CLI11::Module)

target_sources(glue
  PUBLIC
    FILE_SET CXX_MODULES FILES
      src/cli.cppm
      src/glue.cppm
      src/file.cppm
      src/commands/commands.cppm
      src/commands/inspect.cppm
      src/elf/elf.cppm
      src/elf/types.cppm
)
