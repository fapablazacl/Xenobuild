
set (target core-toolchain-clang)

set (sources 
	"include/bok/core/toolchain/clang/ClangArchiver.hpp"
	"include/bok/core/toolchain/clang/ClangCompiler.hpp"
	"include/bok/core/toolchain/clang/ClangLinker.hpp" 
	
	"src/ClangArchiver.cpp"
	"src/ClangCompiler.cpp"
	"src/ClangLinker.cpp" 
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core)
