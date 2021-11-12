cmake_minimum_required(VERSION 3.5)

project (Xenobuild)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Boost
find_package(Boost
    REQUIRED
    COMPONENTS program_options filesystem
)

include_directories(
    "./src"
    ${Boost_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})

include_directories("dependencies/nlohmann-json-3.7.3/include")

find_package(yaml-cpp REQUIRED)

add_subdirectory("src/xenobuild")
add_subdirectory("src/build-core")
add_subdirectory("src/build-core-io")
add_subdirectory("src/build-core-pipeline")
add_subdirectory("src/build-core-package")
add_subdirectory("src/build-core-toolchain")
# NOTE: Temporally removed, waiting for an stable core to build upon on.
# add_subdirectory("src/build-core-toolchain-clang")
# add_subdirectory("src/build-core-toolchain-gcc")
add_subdirectory("src/build-core-toolchain-vc")
add_subdirectory("src/build-feature")
add_subdirectory("src/build-utility")
add_subdirectory("src/xenobuild-test")
