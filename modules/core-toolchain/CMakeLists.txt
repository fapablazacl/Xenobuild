
set (target core-toolchain)

set (sources 
	include/bok/core/toolchain/ArchiveLinker.hpp
	include/bok/core/toolchain/Compiler.hpp
	include/bok/core/toolchain/CompilerMock.hpp
	include/bok/core/toolchain/CompilerVersionDetector.hpp
	include/bok/core/toolchain/Linker.hpp
	include/bok/core/toolchain/LinkerMock.hpp
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
	include/bok/core/toolchain/ToolchainMock.hpp
	include/bok/core/toolchain/ToolchainService.hpp
	include/bok/core/toolchain/ToolchainServiceImpl.hpp
	
	src/ArchiveLinker.cpp
	src/Compiler.cpp
	src/CompilerMock.cpp
	src/CompilerVersionDetector.cpp
	src/Linker.cpp
	src/LinkerMock.cpp
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
	src/ToolchainMock.cpp
	src/ToolchainService.cpp
	src/ToolchainServiceImpl.cpp
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core core-io core-package)
