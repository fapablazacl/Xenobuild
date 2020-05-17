
find_package(Boost
    REQUIRED
    COMPONENTS program_options filesystem
)

include_directories(
    "./src"
    ${Boost_INCLUDE_DIRS}
)

include_directories("../core/")

link_directories(${Boost_LIBRARY_DIRS})

find_package(nlohmann_json)

set (target bok)

set (src "src/Main.cpp")

set (src_borc_cli 
    "src/borc/cli/ControllerStub.hpp"
    "src/borc/cli/Controller.cpp"
    "src/borc/cli/Controller.hpp"
    "src/borc/cli/ControllerFactory.cpp"
    "src/borc/cli/ControllerFactory.hpp"
)

set (src_borc_cli_init
    "src/borc/cli/init/InitController.cpp"
    "src/borc/cli/init/InitController.hpp"
    "src/borc/cli/init/InitControllerOptions.cpp"
    "src/borc/cli/init/InitControllerOptions.hpp"
)

set (src_borc_cli_build
    "src/borc/cli/build/BuildController.cpp"
    "src/borc/cli/build/BuildController.hpp"
    "src/borc/cli/build/BuildControllerOptions.cpp"
    "src/borc/cli/build/BuildControllerOptions.hpp"
)

set (src_borc_cli_configure
    "src/borc/cli/configure/ConfigureController.cpp"
    "src/borc/cli/configure/ConfigureController.hpp"
    "src/borc/cli/configure/ConfigureControllerOptions.cpp"
    "src/borc/cli/configure/ConfigureControllerOptions.hpp"
)

set (src_borc_core
    "src/borc/core/DependencyGraph.hpp"
    "src/borc/core/Core.hpp"
    "src/borc/core/Core.cpp"
)

set (src_borc_common
    "src/borc/common/Constants.hpp"
    "src/borc/common/Constants.cpp"
    "src/borc/common/BuildConfiguration.hpp"
    "src/borc/common/BuildConfiguration.cpp"
)

set (src_borc_model
    "src/borc/model/Context.hpp"
    "src/borc/model/Context.cpp"
    "src/borc/model/Version.hpp"
    "src/borc/model/Source.hpp"
    "src/borc/model/Module.hpp"
    "src/borc/model/ModuleLinkerMatcher.hpp"
    "src/borc/model/SourceCompilerMatcher.hpp"
    "src/borc/model/BuildPipeline.hpp"
    "src/borc/model/Package.hpp"
    "src/borc/model/Package.cpp"
    "src/borc/model/PackageRegistry.hpp"
    "src/borc/model/PackageRegistry.cpp"
    "src/borc/model/PackageRegistryFactory.hpp"
    "src/borc/model/PackageRegistryFactory.cpp"
    "src/borc/model/Command.hpp"
    "src/borc/model/CommandFactory.hpp"
    "src/borc/model/ModuleLinkerMatcher.cpp"
    "src/borc/model/SourceCompilerMatcher.cpp"
    "src/borc/model/BuildPipeline.cpp"
    "src/borc/model/Version.cpp"
    "src/borc/model/Source.cpp"
    "src/borc/model/Module.cpp"
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
    "src/borc/entity/Decoder.hpp"
    "src/borc/entity/Encoder.hpp"
    "src/borc/entity/JsonModel.hpp"
    "src/borc/entity/YamlModel.hpp"
    "src/borc/entity/LanguageEntity.hpp"
    "src/borc/entity/ComponentEntity.hpp"
    "src/borc/entity/PackageEntity.hpp"
    "src/borc/entity/ToolchainEntity.hpp"
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
    "src/borc/toolchain/ToolchainFactory.cpp"
    "src/borc/toolchain/ToolchainFactoryFS.hpp"
    "src/borc/toolchain/ToolchainFactoryFS.cpp"
    "src/borc/toolchain/ToolchainFactoryHardcoded.hpp"
    "src/borc/toolchain/ToolchainFactoryHardcoded.cpp"
    "src/borc/toolchain/ServiceFactory.hpp"
    "src/borc/toolchain/ModuleChecker.hpp"
    "src/borc/toolchain/SourceChecker.hpp"
    "src/borc/toolchain/CompileOptions.cpp"
    "src/borc/toolchain/Compiler.cpp"
    "src/borc/toolchain/CompilerVersionDetector.cpp"
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
    "src/borc/toolchain/ServiceFactory.cpp"
    "src/borc/toolchain/ServiceFactoryStub.hpp"
    "src/borc/toolchain/ServiceFactoryStub.cpp"
    "src/borc/toolchain/ModuleChecker.cpp"
    "src/borc/toolchain/SourceChecker.cpp"
    "src/borc/toolchain/ModuleTypeChecker.hpp"
    "src/borc/toolchain/ModuleTypeChecker.cpp"
    "src/borc/toolchain/ManagedToolchainImpl.cpp"
    "src/borc/toolchain/ManagedToolchainImpl.hpp"
    "src/borc/toolchain/gcc/GCCCompilerVersionDetector.hpp"
    "src/borc/toolchain/gcc/GCCCompilerVersionDetector.cpp"
    "src/borc/toolchain/gcc/ServiceFactoryGCC.hpp"
    "src/borc/toolchain/gcc/ServiceFactoryGCC.cpp"
    "src/borc/toolchain/vc/ServiceFactoryVC.hpp"
    "src/borc/toolchain/vc/ServiceFactoryVC.cpp"
)

set (src_borc_build
    "src/borc/build/ConfigurationService.hpp"
    "src/borc/build/BuildCache.hpp"
    "src/borc/build/ConfigurationService.cpp"
    "src/borc/build/BuildCache.cpp"
    "src/borc/build/BuildCacheTxt.cpp"
    "src/borc/build/BuildCacheTxt.hpp"
)

source_group ("\\src" FILES ${src})
source_group ("\\src\\borc\\cli" FILES ${src_borc_cli})
source_group ("\\src\\borc\\cli\\init" FILES ${src_borc_cli_init})
source_group ("\\src\\borc\\cli\\build" FILES ${src_borc_cli_build})
source_group ("\\src\\borc\\cli\\configure" FILES ${src_borc_cli_configure})
source_group ("\\src\\borc\\core" FILES ${src_borc_core})
source_group ("\\src\\borc\\common" FILES ${src_borc_common})
source_group ("\\src\\borc\\model" FILES ${src_borc_model})
source_group ("\\src\\borc\\model\\command" FILES ${src_borc_model_command})
source_group ("\\src\\borc\\entity" FILES ${src_borc_entity})
source_group ("\\src\\borc\\parsing" FILES ${src_borc_parsing})
source_group ("\\src\\borc\\services" FILES ${src_borc_services})
source_group ("\\src\\borc\\utility" FILES ${src_borc_utility})
source_group ("\\src\\borc\\toolchain" FILES ${src_borc_toolchain})
source_group ("\\src\\borc\\build" FILES ${src_borc_build})

add_executable(${target} 
    ${src}
    ${src_borc_cli}
    ${src_borc_cli_init}
    ${src_borc_cli_build}
    ${src_borc_cli_configure}
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

if (WIN32)
    set (PTHREAD_LIB )
else()
    set (PTHREAD_LIB pthread)
endif()

target_link_libraries(${target} ${Boost_LIBRARIES} ${PTHREAD_LIB} yaml-cpp nlohmann_json)

install(
    TARGETS ${target}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
