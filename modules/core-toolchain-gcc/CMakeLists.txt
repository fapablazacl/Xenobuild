
set (target core-toolchain-gcc)

set (sources 
	"include/bok/toolchain/gcc/GCCCompilerVersionDetector.hpp"
	"include/bok/toolchain/gcc/ServiceFactoryGCC.hpp"
	
	"src/GCCCompilerVersionDetector.cpp"
	"src/ServiceFactoryGCC.cpp" 
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
