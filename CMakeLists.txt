cmake_minimum_required(VERSION 3.8)

project (Xenobuild)

option(XENOBUILD_ENABLE_LINTER "Enable the use of Linter Tool. clang-tidy must be installed." OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Boost
find_package(Boost
    REQUIRED
    COMPONENTS program_options filesystem thread
)

# some definitions
add_definitions(-DCMAKE_CURRENT_SOURCE_DIR="$CMAKE_CURRENT_SOURCE_DIR")

# yaml-cpp
set(YAML_CPP_BUILD_TESTS "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_BUILD_CONTRIB "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_BUILD_TOOLS "OFF" CACHE STRING "" FORCE)
set(YAML_CPP_INSTALL "OFF" CACHE STRING "" FORCE)
add_subdirectory("dependencies/yaml-cpp")

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

add_subdirectory("src/Xenobuild")
add_subdirectory("src/core")
# add_subdirectory("src/build-core")
# add_subdirectory("src/build-core-io")
# add_subdirectory("src/build-core-pipeline")
# add_subdirectory("src/build-core-package")
# add_subdirectory("src/build-core-toolchain")
# add_subdirectory("src/build-core-toolchain-clang")
# add_subdirectory("src/build-core-toolchain-gcc")
# add_subdirectory("src/build-core-toolchain-vc")
# add_subdirectory("src/build-feature")
# add_subdirectory("src/xb-test")
