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

# YAML_BUILD_SHARED_LIBS:BOOL=OFF
# YAML_CPP_BUILD_CONTRIB:BOOL=OFF
# YAML_CPP_BUILD_TESTS:BOOL=OFF
# YAML_CPP_BUILD_TOOLS:BOOL=OFF
# YAML_CPP_INSTALL:BOOL=OFF

include_directories("dependencies/nlohmann-json-3.7.3/include")

add_subdirectory("dependencies/yaml-cpp")
include_directories("dependencies/yaml-cpp/include")

include_directories("component/utility/include")
include_directories("component/core/src")
include_directories("component/feature/src")
include_directories("component/bok/src")

add_subdirectory("component/bok")
add_subdirectory("component/core")
add_subdirectory("component/feature")
add_subdirectory("component/utility")
add_subdirectory("component/bok-test")
