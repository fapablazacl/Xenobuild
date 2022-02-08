
project(HelloWorld)

find_package(yaml-cpp REQUIRED)

add_executable(HelloWorld HelloWorld.cpp)
target_link_libraries(HelloWorld yaml-cpp)
