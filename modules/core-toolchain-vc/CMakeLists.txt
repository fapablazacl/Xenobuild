
set (target core-toolchain-vc)

set (sources 
	"include/bok/core/toolchain/vc/Toolchain_VC.hpp"
	"include/bok/core/toolchain/vc/Linker_VC.hpp"
	"include/bok/core/toolchain/vc/Compiler_VC.hpp"
	
	"src/Toolchain_VC.cpp"
	"src/Linker_VC.cpp"
	"src/Compiler_VC.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core core-toolchain core-package)
