
set (target core-toolchain-gcc)

set (sources 
	"include/bok/core/toolchain/gcc/GCCCompilerVersionDetector.hpp"
	"include/bok/core/toolchain/gcc/ServiceFactoryGCC.hpp"
	
	"src/GCCCompilerVersionDetector.cpp"
	"src/ServiceFactoryGCC.cpp" 
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core core-toolchain core-package)
