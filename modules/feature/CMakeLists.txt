
set (target feature)

set (sources 
    ./include/bok/feature/build/ConfigurationService.hpp
    ./src/ConfigurationService.cpp
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core core-pipeline core-io)
