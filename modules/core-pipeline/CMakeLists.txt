
set (target core-pipeline)

set (sources 
	include/bok/core/pipeline/BuildCache.hpp
	include/bok/core/pipeline/BuildCache_FS.hpp
	include/bok/core/pipeline/BuildConfiguration.hpp
	include/bok/core/pipeline/TaskGraphGenerator.hpp
	include/bok/core/pipeline/TaskGraphGenerator_Build.hpp
	include/bok/core/pipeline/BuildPipeline.hpp
	include/bok/core/pipeline/FileScanner.hpp
	include/bok/core/pipeline/FileScanner_FS.hpp

    src/BuildCache.cpp
	src/BuildCache_FS.cpp
    src/BuildPipeline.cpp
    src/TaskGraphGenerator.cpp
	src/TaskGraphGenerator_Build.cpp
    src/BuildConfiguration.cpp
    src/FileScanner.cpp
    src/FileScanner_FS.cpp
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core core-package)
