
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
	"include/Xenobuild/core/URL.h"
	"include/Xenobuild/core/Util.h"
	"include/Xenobuild/core/FileSystemPackageFactory.h"
	"include/Xenobuild/core/string.h"
	"include/Xenobuild/core/WildcardClassifier.h"
	
	"src/string.cpp"
	"src/WildcardClassifier.cpp"
	"src/Command.cpp"
	"src/Dependency.cpp"
	"src/DependencyManager.cpp"
	"src/FileSystemPackageFactory.cpp"
	"src/PackageFactory.cpp"
	"src/PackageManager.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} yaml-cpp)
