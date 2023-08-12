cmake_minimum_required(VERSION 3.8)

project (Xenobuild)

option(XENOBUILD_ENABLE_LINTER  "Enable the use of Linter Tool. clang-tidy must be installed."  OFF)
option(XENOBUILD_ENABLE_TESTS   "Enable the automated self-tests for all libraries."            OFF)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# some definitions
add_definitions(-DCMAKE_CURRENT_SOURCE_DIR="$CMAKE_CURRENT_SOURCE_DIR")

# yaml-cpp
set(YAML_CPP_BUILD_TESTS    "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_BUILD_CONTRIB  "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_BUILD_TOOLS    "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_INSTALL        "OFF" CACHE STRING "" FORCE)
add_subdirectory("dependencies/yaml-cpp")

# add unit test support libraries
if (XENOBUILD_ENABLE_TESTS)
    add_subdirectory("dependencies/googletest")
    include(CTest)
endif()

if (XENOBUILD_ENABLE_LINTER)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
else ()
    set(CMAKE_CXX_CLANG_TIDY "")
endif()

# Enable Full Warning for all build configuration and compilers
if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_subdirectory("src/core")
add_subdirectory("src/libxenocli")
add_subdirectory("src/xenocli")
