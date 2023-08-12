
include_directories(include)

set (target borc)

set (sources 
    borc/Borc.hpp 
    borc/Borc.cpp)

add_executable(${target} ${sources})

target_link_libraries(${target} libborc)
