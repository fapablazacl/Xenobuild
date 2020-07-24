
set (target core-pipeline)

set (sources 
	./include/bok/core/pipeline/BuildCacheFileSystem.hpp
	./include/bok/core/pipeline/BuildCache.hpp
	./include/bok/core/pipeline/BuildTaskGraphGenerator.hpp
	./include/bok/core/pipeline/DiskFileScanner.hpp
	./include/bok/core/pipeline/FileScanner.hpp
	./include/bok/core/pipeline/BuildConfiguration.hpp
	./include/bok/core/pipeline/TaskGraphGenerator.hpp
	./include/bok/core/pipeline/BuildPipeline.hpp

    ./src/BuildCacheFileSystem.cpp
    ./src/BuildCache.cpp
    ./src/BuildTaskGraphGenerator.cpp
    ./src/BuildPipeline.cpp
    ./src/TaskGraphGenerator.cpp
    ./src/BuildConfiguration.cpp
    ./src/FileScanner.cpp
    ./src/DiskFileScanner.cpp
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
