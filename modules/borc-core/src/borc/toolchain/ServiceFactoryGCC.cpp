
#include "ServiceFactoryGCC.hpp"

#include <borc/model/Command.hpp>
#include <borc/toolchain/SourceChecker.hpp>

#include "ArtifactTypeChecker.hpp"
#include "CompilerImpl.hpp"
#include "LinkerImpl.hpp"
#include "ArchiveLinker.hpp"

namespace borc {
    ServiceFactoryGCC::ServiceFactoryGCC(const std::string &commandBase) {
        this->commandBase = commandBase;

        compilers.push_back({
            std::make_unique<SourceChecker>(std::initializer_list<std::string>({"*.cpp", "*.cxx", "*.c++", "*.cc"})),
            this->createCompiler()
        });

        linkers.push_back({
            std::make_unique<ArtifactTypeChecker>(std::initializer_list<Artifact::Type>({
                Artifact::Type::ApplicationCli, 
                Artifact::Type::ApplicationGui, 
                Artifact::Type::LibraryDynamic
            })), 
            this->createLinker()
        });

        linkers.push_back({
            std::make_unique<ArtifactTypeChecker>(std::initializer_list<Artifact::Type>({
                Artifact::Type::LibraryStatic
            })), 
            this->createStaticLinker()
        });
    }

    ServiceFactoryGCC::~ServiceFactoryGCC() {}

    std::unique_ptr<Compiler> ServiceFactoryGCC::createCompiler() {
        CompilerSwitches switches;
        switches.compile = "-std=c++17 -c";
        switches.includeDebug = "-g";
        switches.zeroOptimization = "-O0";
        switches.objectFileOutput = "-o";
        switches.includePath = "-I";

        CompilerConfiguration configuration;

        return std::make_unique<CompilerImpl> (&commandFactory, commandBase, switches, configuration);
    }

    std::unique_ptr<Linker> ServiceFactoryGCC::createLinker() {
        LinkerSwitches switches;
        switches.buildSharedLibrary = "-shared";
        switches.moduleOutput = "-o";
        switches.importLibrary = "-l";
        switches.importLibraryPath = "-L";

        LinkerConfiguration configuration;
        configuration.importLibraries = {
            "stdc++", "stdc++fs"
        };

        return std::make_unique<LinkerImpl> (&commandFactory, commandBase, switches, configuration);
    }

    std::unique_ptr<Linker> ServiceFactoryGCC::createStaticLinker() {
        ArchiveLinker::Switches switches;

        switches.buildStaticLibrary = "rcs";
        switches.moduleOutput = "";

        return std::make_unique<ArchiveLinker>(&commandFactory, "ar", switches);
    }
}
