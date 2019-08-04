
#include "ServiceFactoryGCC.hpp"

#include <borc/model/Command.hpp>
#include <borc/toolchain/CompilerImpl.hpp>
#include <borc/toolchain/LinkerImpl.hpp>

namespace borc {
    ServiceFactoryGCC::ServiceFactoryGCC(const std::string &commandBase) {
        this->commandBase = commandBase;

        this->compiler = this->createCompiler();
        this->linker = this->createLinker();
    }

    std::unique_ptr<Compiler> ServiceFactoryGCC::createCompiler() {
        CompilerSwitches switches;
        switches.compile = "-c";
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

    const Compiler* ServiceFactoryGCC::getCompiler() const {
        return compiler.get();
    }

    const Linker* ServiceFactoryGCC::getLinker() const {
        return linker.get();
    }
}
