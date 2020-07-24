
set (target core)

set (sources 
	include/bok/core/Command.hpp
	include/bok/core/CommandFactory.hpp
	include/bok/core/ConsoleLogger.hpp
	include/bok/core/Constants.hpp
	include/bok/core/Dag.hpp
	include/bok/core/DagNode.hpp
	include/bok/core/DagVisitor.hpp
	include/bok/core/FileService.hpp
	include/bok/core/FileServiceImpl.hpp
	include/bok/core/FileTypeService.hpp
	include/bok/core/FolderCommand.hpp
	include/bok/core/Logger.hpp
	include/bok/core/Predef.h
	include/bok/core/SystemCommand.hpp
	include/bok/core/Types.hpp
	include/bok/core/Version.hpp
	
	src/Command.cpp
	src/CommandFactory.cpp
	src/ConsoleLogger.cpp
	src/Constants.cpp
	src/Dag.cpp
	src/DagNode.cpp
	src/DagVisitor.cpp
	src/FileService.cpp
	src/FileServiceImpl.cpp
	src/FileTypeService.cpp
	src/FolderCommand.cpp
	src/Logger.cpp
	src/SystemCommand.cpp
	src/Version.cpp
)

add_library(${target} ${sources})

target_link_libraries(${target} yaml-cpp ${Boost_LIBRARIES})

target_include_directories(${target} PUBLIC "include/")
