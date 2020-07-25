
set (target core-toolchain)

set (sources 
	include/bok/core/toolchain/Compiler.hpp
	include/bok/core/toolchain/Compiler_Mock.hpp
	include/bok/core/toolchain/Linker.hpp
	include/bok/core/toolchain/Linker_Mock.hpp
	include/bok/core/toolchain/Linker_Archive.hpp
	include/bok/core/toolchain/Linker_Module.hpp
	include/bok/core/toolchain/Toolchain.hpp
	include/bok/core/toolchain/Toolchain_Mock.hpp
	include/bok/core/toolchain/ToolchainFactory.hpp
	include/bok/core/toolchain/ToolchainFactory_FS.hpp
	include/bok/core/toolchain/ToolchainFactory_Mock.hpp
	include/bok/core/toolchain/ToolchainService.hpp
	include/bok/core/toolchain/ToolchainService_FS.hpp
	
	src/Linker_Archive.cpp
	src/Compiler.cpp
	src/Compiler_Mock.cpp
	src/Linker.cpp
	src/Linker_Mock.cpp
	src/Linker_Module.cpp
	src/Toolchain.cpp
	src/Toolchain_Mock.cpp
	src/ToolchainFactory.cpp
	src/ToolchainFactory_FS.cpp
	src/ToolchainFactory_Mock.cpp
	src/ToolchainService.cpp
	src/ToolchainService_FS.cpp
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core core-io)
