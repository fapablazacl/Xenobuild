cmake_minimum_required(VERSION 3.5)

project (bok)

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

set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory("dependencies/yaml-cpp")
include_directories("dependencies/yaml-cpp/include")

add_subdirectory("modules/bok")
add_subdirectory("modules/core")
add_subdirectory("modules/core-io")
add_subdirectory("modules/core-pipeline")
add_subdirectory("modules/core-package")
add_subdirectory("modules/core-toolchain")
add_subdirectory("modules/core-toolchain-clang")
add_subdirectory("modules/core-toolchain-gcc")
add_subdirectory("modules/core-toolchain-vc")
add_subdirectory("modules/feature")
add_subdirectory("modules/utility")
add_subdirectory("modules/bok-test")
