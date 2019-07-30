
#include "ServiceFactoryGCC.hpp"

#include <borc/toolchain/Command.hpp>
#include <borc/toolchain/Compiler.hpp>
#include <borc/toolchain/Linker.hpp>
#include <borc/toolchain/BuildService.hpp>
#include <borc/toolchain/RunService.hpp>

namespace borc {
    ServiceFactoryGCC::ServiceFactoryGCC(const std::string &commandBase) {
        this->commandBase = commandBase;

        this->compiler = this->createCompiler();
        this->linker = this->createLinker();
    }

    BuildService ServiceFactoryGCC::createBuildService() {
        return BuildService{ compiler.get(), linker.get() };
    }

    RunService ServiceFactoryGCC::createRunService() {
        return RunService{ compiler.get(), linker.get() };
    }

    std::unique_ptr<Compiler> ServiceFactoryGCC::createCompiler() {
        CompilerSwitches switches;
        switches.compile = "-c";
        switches.includeDebug = "-g";
        switches.zeroOptimization = "-O0";
        switches.objectFileOutput = "-o";
        switches.includePath = "-I";

        CompilerConfiguration configuration;

        return std::make_unique<Compiler> (&commandFactory, commandBase, switches, configuration);
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

        return std::make_unique<Linker> (&commandFactory, commandBase, switches, configuration);
    }

    const Compiler* ServiceFactoryGCC::getCompiler() const {
        return compiler.get();
    }

    const Linker* ServiceFactoryGCC::getLinker() const {
        return linker.get();
    }
}
