
set (target borc-tdd)

set (sources 
    borc/Main.cpp 
    borc/TestParser.cpp
)

add_executable(${target} ${sources})

target_link_libraries(${target} libborc UnitTest++)
