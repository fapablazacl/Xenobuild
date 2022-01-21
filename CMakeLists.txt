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

add_definitions(-DCMAKE_CURRENT_SOURCE_DIR="$CMAKE_CURRENT_SOURCE_DIR")

include_directories(
    "./src"
    ${Boost_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})

add_subdirectory("src/Xenobuild")
add_subdirectory("src/core")
add_subdirectory("src/build-core")

# add_subdirectory("src/build-core-io")
# add_subdirectory("src/build-core-pipeline")
# add_subdirectory("src/build-core-package")
# add_subdirectory("src/build-core-toolchain")
# add_subdirectory("src/build-core-toolchain-clang")
# add_subdirectory("src/build-core-toolchain-gcc")
# add_subdirectory("src/build-core-toolchain-vc")
# add_subdirectory("src/build-feature")
# add_subdirectory("src/xb-test")
