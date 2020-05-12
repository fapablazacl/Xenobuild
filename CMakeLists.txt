cmake_minimum_required(VERSION 3.5)

project (borc)

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

add_subdirectory("component/borc")
