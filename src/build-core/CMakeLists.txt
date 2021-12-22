
set (target build-core)

set (sources 
	"include/Xenobuild/core/Predef.h"
	"include/Xenobuild/core/Command.h"
	"include/Xenobuild/core/Command_System.h"
	"include/Xenobuild/core/Command_Folder.h"
	"include/Xenobuild/core/CommandFactory.h"
	"include/Xenobuild/core/Constants.h"
	"include/Xenobuild/core/Dag.h"
	"include/Xenobuild/core/DagNode.h"
	"include/Xenobuild/core/DagVisitor.h"
	"include/Xenobuild/core/FileService.h"
	"include/Xenobuild/core/FileService_FS.h"
	"include/Xenobuild/core/FileTypeService.h"
	"include/Xenobuild/core/Logger.h"
	"include/Xenobuild/core/Logger_Console.h"
	"include/Xenobuild/core/Types.h"
	"include/Xenobuild/core/Version.h"
	
	"src/Command.cpp"
	"src/Command_System.cpp"
	"src/Command_Folder.cpp"
	"src/CommandFactory.cpp"
	"src/Constants.cpp"
	"src/Dag.cpp"
	"src/DagNode.cpp"
	"src/DagVisitor.cpp"
	"src/FileService.cpp"
	"src/FileService_FS.cpp"
	"src/FileTypeService.cpp"
	"src/Logger.cpp"
	"src/Logger_Console.cpp"
	"src/Version.cpp"
)

add_library(${target} ${sources})

target_link_libraries(${target} ${Boost_LIBRARIES})

target_include_directories(${target} PUBLIC "include/")
