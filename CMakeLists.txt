cmake_minimum_required(VERSION 3.5)

project (bok)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# YAML_BUILD_SHARED_LIBS:BOOL=OFF
# YAML_CPP_BUILD_CONTRIB:BOOL=OFF
# YAML_CPP_BUILD_TESTS:BOOL=OFF
# YAML_CPP_BUILD_TOOLS:BOOL=OFF
# YAML_CPP_INSTALL:BOOL=OFF

add_subdirectory("dependencies/yaml-cpp")
include_directories("dependencies/yaml-cpp/include")

include_directories("component/core/src")
include_directories("component/feature/src")
include_directories("component/utility/src")
include_directories("component/bok/src")

add_subdirectory("component/bok")
add_subdirectory("component/core")
add_subdirectory("component/feature")
add_subdirectory("component/utility")
