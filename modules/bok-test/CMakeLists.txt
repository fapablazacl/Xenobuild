
set (target bok-test)
set (sources src/main.cpp)

add_definitions(-DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

source_group(\\ FILES ${sources})

add_executable(${target} ${sources})

target_link_libraries(${target} core core-pipeline feature utility)
