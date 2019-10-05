
set (target borc-core-cpp)
set (sources 


)

add_library(${target} ${sources})
target_link_libraries(${target} borc-core)
