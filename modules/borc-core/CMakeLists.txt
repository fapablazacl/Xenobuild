
include_directories("../borc-core/src")

find_package(Boost
    REQUIRED
    COMPONENTS filesystem
)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(nlohmann_json REQUIRED)

set (src_borc_core
    "src/borc/core/DependencyGraph.hpp"
)

set (src_borc_common
    "src/borc/common/BuildConfiguration.hpp"
    "src/borc/common/BuildConfiguration.cpp"
)

set (src_borc_model
    "src/borc/model/Version.hpp"
    "src/borc/model/Source.hpp"
    "src/borc/model/Module.hpp"
    "src/borc/model/Package.hpp"
    "src/borc/model/ModuleLinkerMatcher.hpp"
    "src/borc/model/SourceCompilerMatcher.hpp"
    "src/borc/model/BuildPipeline.hpp"
    "src/borc/model/Context.hpp"
    "src/borc/model/Command.hpp"
    "src/borc/model/CommandFactory.hpp"
    "src/borc/model/ModuleLinkerMatcher.cpp"
    "src/borc/model/SourceCompilerMatcher.cpp"
    "src/borc/model/BuildPipeline.cpp"
    "src/borc/model/Version.cpp"
    "src/borc/model/Source.cpp"
    "src/borc/model/Module.cpp"
    "src/borc/model/Package.cpp"
    "src/borc/model/Context.cpp"
    "src/borc/model/Command.cpp"
    "src/borc/model/CommandFactory.cpp"
)

set (src_borc_model_command
    "src/borc/model/command/SystemCommand.hpp"
    "src/borc/model/command/SystemCommand.cpp"
    "src/borc/model/command/FolderCommand.hpp"
    "src/borc/model/command/FolderCommand.cpp"
)

set (src_borc_entity
    "src/borc/entity/LanguageEntity.hpp"
    "src/borc/entity/ModuleEntity.hpp"
    "src/borc/entity/PackageEntity.hpp"
    "src/borc/entity/ToolchainEntity.hpp"
)

set (src_borc_parsing
    "src/borc/parsing/JSONDeserializer.hpp"
    "src/borc/parsing/JSONSerializer.hpp"
)

set (src_borc_services
    "src/borc/services/PackageService.hpp"
    "src/borc/services/PackageService.cpp"
    "src/borc/services/PackageServiceImpl.hpp"
    "src/borc/services/PackageServiceImpl.cpp"
    "src/borc/services/FileService.hpp"
    "src/borc/services/FileServiceImpl.hpp"
    "src/borc/services/LoggingService.hpp"
    "src/borc/services/LoggingServiceImpl.hpp"
    "src/borc/services/BuildService.hpp"
    "src/borc/services/BuildServiceImpl.hpp"
    "src/borc/services/BuildService.cpp"
    "src/borc/services/BuildServiceImpl.cpp"
    "src/borc/services/FileService.cpp"
    "src/borc/services/FileServiceImpl.cpp"
    "src/borc/services/LoggingService.cpp"
    "src/borc/services/LoggingServiceImpl.cpp"
    "src/borc/services/ToolchainService.hpp"
    "src/borc/services/ToolchainService.cpp"
    "src/borc/services/ToolchainServiceImpl.hpp"
    "src/borc/services/ToolchainServiceImpl.cpp"
)

set (src_borc_utility
    "src/borc/utility/Dag.hpp"
    "src/borc/utility/DagNode.hpp"
    "src/borc/utility/DagVisitor.hpp"
    "src/borc/utility/Dag.cpp"
    "src/borc/utility/DagNode.cpp"
    "src/borc/utility/DagVisitor.cpp"
)

set (src_borc_toolchain
    "src/borc/toolchain/CompileOptions.hpp"
    "src/borc/toolchain/Compiler.hpp"
    "src/borc/toolchain/CompilerVersionDetector.hpp"
    "src/borc/toolchain/Toolchain.hpp"
    "src/borc/toolchain/ToolchainFactory.hpp"
    "src/borc/toolchain/ServiceFactory.hpp"
    "src/borc/toolchain/ModuleChecker.hpp"
    "src/borc/toolchain/SourceChecker.hpp"
    "src/borc/toolchain/CompileOptions.cpp"
    "src/borc/toolchain/Compiler.cpp"
    "src/borc/toolchain/CompilerVersionDetector.cpp"
    "src/borc/toolchain/GCCCompilerVersionDetector.hpp"
    "src/borc/toolchain/GCCCompilerVersionDetector.cpp"
    "src/borc/toolchain/CompilerImpl.hpp"
    "src/borc/toolchain/CompilerImpl.cpp"
    "src/borc/toolchain/Linker.hpp"
    "src/borc/toolchain/Linker.cpp"
    "src/borc/toolchain/LinkerImpl.hpp"
    "src/borc/toolchain/LinkerImpl.cpp"
    "src/borc/toolchain/ArchiveLinker.cpp"
    "src/borc/toolchain/ArchiveLinker.cpp"
    "src/borc/toolchain/ModuleLinker.hpp"
    "src/borc/toolchain/ModuleLinker.cpp"
    "src/borc/toolchain/Toolchain.cpp"
    "src/borc/toolchain/ToolchainImpl.hpp"
    "src/borc/toolchain/ToolchainImpl.cpp"
    "src/borc/toolchain/ToolchainFactory.cpp"
    "src/borc/toolchain/ServiceFactory.cpp"
    "src/borc/toolchain/ServiceFactoryStub.hpp"
    "src/borc/toolchain/ServiceFactoryStub.cpp"
    "src/borc/toolchain/ServiceFactoryGCC.hpp"
    "src/borc/toolchain/ServiceFactoryGCC.cpp"
    "src/borc/toolchain/ServiceFactoryVC.hpp"
    "src/borc/toolchain/ServiceFactoryVC.cpp"
    "src/borc/toolchain/ModuleChecker.cpp"
    "src/borc/toolchain/SourceChecker.cpp"
    "src/borc/toolchain/ModuleTypeChecker.hpp"
    "src/borc/toolchain/ModuleTypeChecker.cpp"
    "src/borc/toolchain/ManagedToolchainImpl.cpp"
    "src/borc/toolchain/ManagedToolchainImpl.hpp"
)

set (src_borc_build
    "src/borc/build/ConfigurationService.hpp"
    "src/borc/build/BuildCache.hpp"
    "src/borc/build/ConfigurationService.cpp"
    "src/borc/build/BuildCache.cpp"
    "src/borc/build/BuildCacheImpl.cpp"
    "src/borc/build/BuildCacheImpl.hpp"
)

source_group("\\src\\borc\\core" FILES ${src_borc_core})
source_group("\\src\\borc\\common" FILES ${src_borc_common})
source_group("\\src\\borc\\model" FILES ${src_borc_model})
source_group("\\src\\borc\\model\\command" FILES ${src_borc_model_command})
source_group("\\src\\borc\\entity" FILES ${src_borc_entity})
source_group("\\src\\borc\\parsing" FILES ${src_borc_parsing})
source_group("\\src\\borc\\services" FILES ${src_borc_services})
source_group("\\src\\borc\\utility" FILES ${src_borc_utility})
source_group("\\src\\borc\\toolchain" FILES ${src_borc_toolchain})
source_group("\\src\\borc\\build" FILES ${src_borc_build})

add_library(borc-core 
    ${src_borc_core}
    ${src_borc_common}
    ${src_borc_model}
    ${src_borc_model_command}
    ${src_borc_entity}
    ${src_borc_parsing}
    ${src_borc_services}
    ${src_borc_utility}
    ${src_borc_toolchain}
    ${src_borc_build}
)

target_link_libraries(borc-core ${Boost_LIBRARIES} nlohmann_json::nlohmann_json)
