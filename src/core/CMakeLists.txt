
set (target "core")

set (sources 
	"include/Xenobuild/core/SourceFile.h"
	"include/Xenobuild/core/Module.h"
	"include/Xenobuild/core/Package.h"
	"include/Xenobuild/core/PackageFactory.h"
	"include/Xenobuild/core/FileSystemPackageFactory.h"
	"include/Xenobuild/core/string.h"
	"include/Xenobuild/core/WildcardClassifier.h"
	
	"src/FileSystemPackageFactory.cpp"
	"src/string.cpp"
	"src/WildcardClassifier.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
