
set (target "core")

set (sources 
	"include/Xenobuild/core/Command.h"
	"include/Xenobuild/core/Dependency.h"
	"include/Xenobuild/core/DependencyManager.h"
	"include/Xenobuild/core/SourceFile.h"
	"include/Xenobuild/core/Module.h"
	"include/Xenobuild/core/Package.h"
	"include/Xenobuild/core/PackageFactory.h"
	"include/Xenobuild/core/PackageManager.h"
	"include/Xenobuild/core/Triplet.h"
	"include/Xenobuild/core/Toolchain.h"
	"include/Xenobuild/core/ToolchainFactory.h"
	"include/Xenobuild/core/URL.h"
	"include/Xenobuild/core/Util.h"
	"include/Xenobuild/core/FileSystemPackageFactory.h"
	"include/Xenobuild/core/string.h"
	"include/Xenobuild/core/WildcardClassifier.h"
	"include/Xenobuild/core/CMakeBuildSystem.h"
	"include/Xenobuild/core/GitRepository.h"
	"include/Xenobuild/core/ToolchainCommandExecutor.h"
	
	"src/string.cpp"
	"src/WildcardClassifier.cpp"
	"src/Command.cpp"
	"src/Dependency.cpp"
	"src/DependencyManager.cpp"
	"src/Util.cpp"
	"src/FileSystemPackageFactory.cpp"
	"src/PackageFactory.cpp"
	"src/PackageManager.cpp"
	"src/Package.cpp"
	"src/Toolchain.cpp"
	"src/ToolchainFactory.cpp"
	"src/CMakeBuildSystem.cpp"
	"src/GitRepository.cpp"
	"src/ToolchainCommandExecutor.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} yaml-cpp)
