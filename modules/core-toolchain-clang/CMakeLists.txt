
set (target core-toolchain-clang)

set (sources 
	"include/bok/toolchain/clang/ClangArchiver.hpp"
	"include/bok/toolchain/clang/ClangCompiler.hpp"
	"include/bok/toolchain/clang/ClangLinker.hpp" 
	
	"src/ClangArchiver.cpp"
	"src/ClangCompiler.cpp"
	"src/ClangLinker.cpp" 
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
