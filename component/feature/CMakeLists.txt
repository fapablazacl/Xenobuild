
set (target feature)

set (sources 
    ./src/bok/feature/build/BuildCacheTxt.cpp
    ./src/bok/feature/build/ConfigurationService.hpp
    ./src/bok/feature/build/BuildCache.cpp
    ./src/bok/feature/build/ConfigurationService.cpp
    ./src/bok/feature/build/BuildCache.hpp
    ./src/bok/feature/build/BuildCacheTxt.hpp
    ./src/bok/feature/sync/SourceScanner.hpp
    ./src/bok/feature/sync/SourceScanner.cpp
)

add_library(${target} ${sources})
