
set (target core-toolchain)

set (sources 
	include/bok/core/toolchain/ArchiveLinker.hpp
	include/bok/core/toolchain/CompileOptions.hpp
	include/bok/core/toolchain/Compiler.hpp
	include/bok/core/toolchain/CompilerImpl.hpp
	include/bok/core/toolchain/CompilerVersionDetector.hpp
	include/bok/core/toolchain/Linker.hpp
	include/bok/core/toolchain/LinkerImpl.hpp
	include/bok/core/toolchain/ManagedToolchainImpl.hpp
	include/bok/core/toolchain/ModuleChecker.hpp
	include/bok/core/toolchain/ModuleLinker.hpp
	include/bok/core/toolchain/ModuleTypeChecker.hpp
	include/bok/core/toolchain/ServiceFactory.hpp
	include/bok/core/toolchain/ServiceFactoryStub.hpp
	include/bok/core/toolchain/SourceChecker.hpp
	include/bok/core/toolchain/Toolchain.hpp
	include/bok/core/toolchain/ToolchainFactory.hpp
	include/bok/core/toolchain/ToolchainFactoryFS.hpp
	include/bok/core/toolchain/ToolchainFactoryHardcoded.hpp
	include/bok/core/toolchain/ToolchainImpl.hpp
	include/bok/core/toolchain/ToolchainService.hpp
	include/bok/core/toolchain/ToolchainServiceImpl.hpp
	
	src/ArchiveLinker.cpp
	src/CompileOptions.cpp
	src/Compiler.cpp
	src/CompilerImpl.cpp
	src/CompilerVersionDetector.cpp
	src/Linker.cpp
	src/LinkerImpl.cpp
	src/ManagedToolchainImpl.cpp
	src/ModuleChecker.cpp
	src/ModuleLinker.cpp
	src/ModuleTypeChecker.cpp
	src/ServiceFactory.cpp
	src/ServiceFactoryStub.cpp
	src/SourceChecker.cpp
	src/Toolchain.cpp
	src/ToolchainFactory.cpp
	src/ToolchainFactoryFS.cpp
	src/ToolchainFactoryHardcoded.cpp
	src/ToolchainImpl.cpp
	src/ToolchainService.cpp
	src/ToolchainServiceImpl.cpp
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
