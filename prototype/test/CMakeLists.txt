cmake_minimum_required(VERSION 3.1)

project(Xenobuild)

set (CMAKE_CXX_STANDARD 14)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(yaml-cpp REQUIRED)

add_subdirectory("src/Xenobuild")
