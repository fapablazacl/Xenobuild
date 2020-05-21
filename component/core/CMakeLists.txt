# Boost
find_package(Boost
    REQUIRED
    COMPONENTS program_options filesystem
)

include_directories(
    "./src"
    ${Boost_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})


# NLOHMANN JSON
find_package(nlohmann_json)

set (target core)

set (sources 
    ./src/bok/core/Predef.h
    ./src/bok/core/common/DagNode.hpp
    ./src/bok/core/common/Dag.hpp
    ./src/bok/core/common/DagVisitor.hpp
    ./src/bok/core/common/Dag.cpp
    ./src/bok/core/common/DagVisitor.cpp
    ./src/bok/core/common/DagNode.cpp
    ./src/bok/core/common/Constants.hpp
    ./src/bok/core/common/LoggingService.hpp
    ./src/bok/core/common/FileService.cpp
    ./src/bok/core/common/LoggingServiceImpl.hpp
    ./src/bok/core/common/FileTypeService.hpp
    ./src/bok/core/common/LoggingServiceImpl.cpp
    ./src/bok/core/common/Version.cpp
    ./src/bok/core/common/CommandFactory.cpp
    ./src/bok/core/common/FileServiceImpl.hpp
    ./src/bok/core/common/CommandFactory.hpp
    ./src/bok/core/common/Command.cpp
    ./src/bok/core/common/Command.hpp
    ./src/bok/core/common/Version.hpp
    ./src/bok/core/common/FileService.hpp
    ./src/bok/core/common/Constants.cpp
    ./src/bok/core/common/FileTypeService.cpp
    ./src/bok/core/common/LoggingService.cpp
    ./src/bok/core/common/Types.hpp
    ./src/bok/core/common/FileServiceImpl.cpp
    ./src/bok/core/common/command/FolderCommand.cpp
    ./src/bok/core/common/command/FolderCommand.hpp
    ./src/bok/core/common/command/SystemCommand.cpp
    ./src/bok/core/common/command/SystemCommand.hpp
    ./src/bok/core/toolchain/ServiceFactoryStub.hpp
    ./src/bok/core/toolchain/ArchiveLinker.hpp
    ./src/bok/core/toolchain/gcc/ServiceFactoryGCC.hpp
    ./src/bok/core/toolchain/gcc/ServiceFactoryGCC.cpp
    ./src/bok/core/toolchain/gcc/GCCCompilerVersionDetector.cpp
    ./src/bok/core/toolchain/gcc/GCCCompilerVersionDetector.hpp
    ./src/bok/core/toolchain/Compiler.cpp
    ./src/bok/core/toolchain/ToolchainFactoryFS.hpp
    ./src/bok/core/toolchain/ModuleChecker.cpp
    ./src/bok/core/toolchain/CompileOptions.cpp
    ./src/bok/core/toolchain/ToolchainFactory.hpp
    ./src/bok/core/toolchain/ToolchainServiceImpl.hpp
    ./src/bok/core/toolchain/ToolchainService.hpp
    ./src/bok/core/toolchain/Toolchain.hpp
    ./src/bok/core/toolchain/CompilerImpl.cpp
    ./src/bok/core/toolchain/clang/ClangLinker.cpp
    ./src/bok/core/toolchain/clang/ClangArchiver.cpp
    ./src/bok/core/toolchain/clang/ClangArchiver.hpp
    ./src/bok/core/toolchain/clang/ClangCompiler.hpp
    ./src/bok/core/toolchain/clang/ClangCompiler.cpp
    ./src/bok/core/toolchain/clang/ClangLinker.hpp
    ./src/bok/core/toolchain/CompileOptions.hpp
    ./src/bok/core/toolchain/Linker.cpp
    ./src/bok/core/toolchain/ManagedToolchainImpl.hpp
    ./src/bok/core/toolchain/LinkerImpl.hpp
    ./src/bok/core/toolchain/ModuleLinker.hpp
    ./src/bok/core/toolchain/ToolchainImpl.hpp
    ./src/bok/core/toolchain/LinkerImpl.cpp
    ./src/bok/core/toolchain/ToolchainImpl.cpp
    ./src/bok/core/toolchain/Linker.hpp
    ./src/bok/core/toolchain/ModuleTypeChecker.hpp
    ./src/bok/core/toolchain/ToolchainServiceImpl.cpp
    ./src/bok/core/toolchain/ModuleChecker.hpp
    ./src/bok/core/toolchain/ServiceFactory.cpp
    ./src/bok/core/toolchain/ArchiveLinker.cpp
    ./src/bok/core/toolchain/CompilerImpl.hpp
    ./src/bok/core/toolchain/ToolchainService.cpp
    ./src/bok/core/toolchain/ToolchainFactoryFS.cpp
    ./src/bok/core/toolchain/vc/ServiceFactoryVC.cpp
    ./src/bok/core/toolchain/vc/ServiceFactoryVC.hpp
    ./src/bok/core/toolchain/ServiceFactoryStub.cpp
    ./src/bok/core/toolchain/ToolchainFactory.cpp
    ./src/bok/core/toolchain/ToolchainFactoryHardcoded.cpp
    ./src/bok/core/toolchain/ServiceFactory.hpp
    ./src/bok/core/toolchain/CompilerVersionDetector.cpp
    ./src/bok/core/toolchain/SourceChecker.cpp
    ./src/bok/core/toolchain/ModuleLinker.cpp
    ./src/bok/core/toolchain/ModuleTypeChecker.cpp
    ./src/bok/core/toolchain/Compiler.hpp
    ./src/bok/core/toolchain/SourceChecker.hpp
    ./src/bok/core/toolchain/CompilerVersionDetector.hpp
    ./src/bok/core/toolchain/ToolchainFactoryHardcoded.hpp
    ./src/bok/core/toolchain/Toolchain.cpp
    ./src/bok/core/toolchain/ManagedToolchainImpl.cpp
    ./src/bok/core/pipeline/BuildCacheTxt.cpp
    ./src/bok/core/pipeline/BuildCacheTxt.hpp
    ./src/bok/core/pipeline/BuildCache.cpp
    ./src/bok/core/pipeline/BuildCache.hpp
    ./src/bok/core/pipeline/BuildServiceImpl.cpp
    ./src/bok/core/pipeline/BuildServiceImpl.hpp
    ./src/bok/core/pipeline/BuildPipeline.cpp
    ./src/bok/core/pipeline/BuildPipeline.hpp
    ./src/bok/core/pipeline/BuildService.hpp
    ./src/bok/core/pipeline/BuildService.cpp
    ./src/bok/core/pipeline/BuildConfiguration.cpp
    ./src/bok/core/pipeline/BuildConfiguration.hpp
    ./src/bok/core/io/LanguageEntity.hpp
    ./src/bok/core/io/Decoder.hpp
    ./src/bok/core/io/YamlModel.hpp
    ./src/bok/core/io/PackageEntity.hpp
    ./src/bok/core/io/ComponentEntity.hpp
    ./src/bok/core/io/Encoder.hpp
    ./src/bok/core/io/ToolchainEntity.hpp
    ./src/bok/core/io/JsonModel.hpp
    ./src/bok/core/package/PackageService.cpp
    ./src/bok/core/package/Package.cpp
    ./src/bok/core/package/PackageServiceImpl.hpp
    ./src/bok/core/package/Module.cpp
    ./src/bok/core/package/Module.hpp
    ./src/bok/core/package/Source.cpp
    ./src/bok/core/package/Source.hpp
    ./src/bok/core/package/ModuleLinkerMatcher.cpp
    ./src/bok/core/package/SourceCompilerMatcher.hpp
    ./src/bok/core/package/PackageRegistryFactory.hpp
    ./src/bok/core/package/PackageRegistry.hpp
    ./src/bok/core/package/PackageService.hpp
    ./src/bok/core/package/PackageRegistry.cpp
    ./src/bok/core/package/Context.cpp
    ./src/bok/core/package/PackageRegistryFactory.cpp
    ./src/bok/core/package/Context.hpp
    ./src/bok/core/package/Package.hpp
    ./src/bok/core/package/Language.hpp
    ./src/bok/core/package/SourceCompilerMatcher.cpp
    ./src/bok/core/package/PackageServiceImpl.cpp
    ./src/bok/core/package/ModuleLinkerMatcher.hpp
)

add_library(${target} ${sources})

target_link_libraries(${target} yaml-cpp nlohmann_json ${Boost_LIBRARIES})
