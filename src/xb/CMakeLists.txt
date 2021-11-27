
set (target xb)

set (sources "src/main.cpp")

add_executable(${target} ${sources})
target_link_libraries(${target} xb-core)

# target_link_libraries(${target}  ${Boost_LIBRARIES})
