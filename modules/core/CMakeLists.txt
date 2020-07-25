
set (target core)

set (sources 
	include/bok/core/Predef.h
	include/bok/core/Command.hpp
	include/bok/core/Command_System.hpp
	include/bok/core/Command_Folder.hpp
	include/bok/core/CommandFactory.hpp
	include/bok/core/Constants.hpp
	include/bok/core/Dag.hpp
	include/bok/core/DagNode.hpp
	include/bok/core/DagVisitor.hpp
	include/bok/core/FileService.hpp
	include/bok/core/FileService_FS.hpp
	include/bok/core/FileTypeService.hpp
	include/bok/core/Logger.hpp
	include/bok/core/Logger_Console.hpp
	include/bok/core/Types.hpp
	include/bok/core/Version.hpp
	
	src/Command.cpp
	src/Command_System.cpp
	src/Command_Folder.cpp
	src/CommandFactory.cpp
	src/Constants.cpp
	src/Dag.cpp
	src/DagNode.cpp
	src/DagVisitor.cpp
	src/FileService.cpp
	src/FileService_FS.cpp
	src/FileTypeService.cpp
	src/Logger.cpp
	src/Logger_Console.cpp
	src/Version.cpp
)

add_library(${target} ${sources})

target_link_libraries(${target} yaml-cpp ${Boost_LIBRARIES})

target_include_directories(${target} PUBLIC "include/")
