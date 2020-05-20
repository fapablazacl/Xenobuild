
#include "ServiceFactoryGCC.hpp"

#include <bok/model/Command.hpp>
#include <bok/toolchain/SourceChecker.hpp>

#include <bok/toolchain/ModuleTypeChecker.hpp>
#include <bok/toolchain/CompilerImpl.hpp>
#include <bok/toolchain/ModuleLinker.hpp>
#include <bok/toolchain/ArchiveLinker.hpp>

namespace bok {
    ServiceFactoryGCC::ServiceFactoryGCC(const std::string &commandBase) {
        this->commandBase = commandBase;

        compilers.push_back({
            std::make_unique<SourceChecker>(std::initializer_list<std::string>({"*.cpp", "*.cxx", "*.c++", "*.cc"})),
            this->createCompiler()
        });

        linkers.push_back({
            std::make_unique<ModuleTypeChecker>(std::initializer_list<Module::Type>({
                Module::Type{"application", "cli"},
                Module::Type{"application", "gui"},
                Module::Type{"library", "dynamic"}
            })), 
            this->createLinker()
        });

        linkers.push_back({
            std::make_unique<ModuleTypeChecker>(std::initializer_list<Module::Type>({
                Module::Type{"library", "static"}
            })), 
            this->createStaticLinker()
        });
    }

    ServiceFactoryGCC::~ServiceFactoryGCC() {}

    std::unique_ptr<Compiler> ServiceFactoryGCC::createCompiler() {
        // CompilerSwitches switches;
        // switches.compile = "-std=c++17 -c";
        // switches.includeDebug = "-g";
        // switches.zeroOptimization = "-O0";
        // switches.objectFileOutput = "-o";
        // switches.includePath = "-I";
        // switches.generateDependencies = "-MM -MG";
        // 
        // CompilerConfiguration configuration;
        // 
        // return std::make_unique<CompilerImpl> (&commandFactory, commandBase, switches, configuration);

        return {};
    }

    std::unique_ptr<Linker> ServiceFactoryGCC::createLinker() {
        LinkerSwitches switches;
        switches.buildSharedLibrary = "-shared";
        switches.moduleOutput = "-o";
        switches.importLibrary = "-l";
        switches.importLibraryPath = "-L";

        LinkerConfiguration configuration;
        configuration.importLibraries = {
            "stdc++"
            // , "stdc++fs"
        };

        return std::make_unique<ModuleLinker> (&commandFactory, commandBase, switches, configuration);
    }

    std::unique_ptr<Linker> ServiceFactoryGCC::createStaticLinker() {
        ArchiveLinker::Switches switches;

        switches.buildStaticLibrary = "rcs";
        switches.moduleOutput = "";

        return std::make_unique<ArchiveLinker>(&commandFactory, "ar", switches);
    }
}
