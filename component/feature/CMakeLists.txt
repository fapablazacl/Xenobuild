
set (target feature)

set (sources 
    ./src/bok/feature/build/ConfigurationService.hpp
    ./src/bok/feature/build/ConfigurationService.cpp
    ./src/bok/feature/sync/SourceScanner.hpp
    ./src/bok/feature/sync/SourceScanner.cpp
)

add_library(${target} ${sources})
target_link_libraries(${target} core)
