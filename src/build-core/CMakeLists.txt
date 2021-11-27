
set (target xb-core)

set (sources 
	include/xb/core/Predef.h
	include/xb/core/Command.h
	include/xb/core/Command_System.h
	include/xb/core/Command_Folder.h
	include/xb/core/CommandFactory.h
	include/xb/core/Constants.h
	include/xb/core/Dag.h
	include/xb/core/DagNode.h
	include/xb/core/DagVisitor.h
	include/xb/core/FileService.h
	include/xb/core/FileService_FS.h
	include/xb/core/FileTypeService.h
	include/xb/core/Logger.h
	include/xb/core/Logger_Console.h
	include/xb/core/Types.h
	include/xb/core/Version.h
	
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

target_link_libraries(${target} ${Boost_LIBRARIES})

target_include_directories(${target} PUBLIC "include/")
