
cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

find_package(Catch2 REQUIRED)

add_executable(HelloWorld HelloWorld.cpp)
target_link_libraries(HelloWorld catch2)
