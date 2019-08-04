
#include "ServiceFactoryVC.hpp"

#include <borc/model/Command.hpp>
#include <borc/toolchain/CompilerImpl.hpp>
#include <borc/toolchain/LinkerImpl.hpp>


namespace borc {
    ServiceFactoryVC::ServiceFactoryVC(const std::string &installationPath, const std::string &windowsKitPath) {
        const std::string commandBasePath = installationPath + "bin\\Hostx64\\x64\\";
        const std::string compilerCommand = commandBasePath + "cl.exe";
        const std::string linkerCommand = commandBasePath + "link.exe";

        this->compiler = this->createCompiler(compilerCommand, installationPath, windowsKitPath);
        this->linker = this->createLinker(linkerCommand, installationPath, windowsKitPath);
    }

    ServiceFactoryVC::~ServiceFactoryVC() {}

    const Compiler* ServiceFactoryVC::getCompiler() const {
        return compiler.get();
    }

    const Linker* ServiceFactoryVC::getLinker() const  {
        return linker.get();
    }

    std::unique_ptr<Compiler> ServiceFactoryVC::createCompiler(const std::string &compilerCommand, const std::string &installationPath, const std::string &windowsKitPath) {
        const std::string standardIncludePath = installationPath + "include";
        const std::string ucrtIncludePath = windowsKitPath + "Include\\10.0.17763.0\\ucrt";
        const std::string umIncludePath = windowsKitPath + "Include\\10.0.17763.0\\um";

        const std::string sharedIncludePath = windowsKitPath + "Include\\10.0.17763.0\\shared";

        CompilerSwitches compilerSwitches;
        compilerSwitches.compile = "/c";
        compilerSwitches.objectFileOutput = "/Fo";
        compilerSwitches.zeroOptimization = "/Od";
        compilerSwitches.includePath = "/I";
        compilerSwitches.includeDebug = "/DEBUG:FULL";

        return std::make_unique<CompilerImpl> (
            &commandFactory, compilerCommand, compilerSwitches,
            CompilerConfiguration { 
                {"/EHsc", "/std:c++17"}, 
                { 
                    "\"" + standardIncludePath + "\"", 
                    "\"" + ucrtIncludePath + "\"",
                    "\"" + umIncludePath + "\"",
                    "\"" + sharedIncludePath + "\""
                }
            }
        );
    }

    std::unique_ptr<Linker> ServiceFactoryVC::createLinker(const std::string &linkerCommand, const std::string &installationPath, const std::string &windowsKitPath) {
        LinkerSwitches linkerSwitches;
        linkerSwitches.buildSharedLibrary = "/DLL";
        linkerSwitches.moduleOutput = "/OUT:";
        linkerSwitches.importLibrary = "/IMPLIB:";
        linkerSwitches.importLibraryPath = "/LIBPATH:";

        LinkerConfiguration linkerConfiguration;
        linkerConfiguration.importLibraryPaths = {
            "\"" + installationPath + "lib\\x64" + "\"",
            "\"" + windowsKitPath + "Lib\\10.0.17763.0\\um\\x64" + "\"",
            "\"" + windowsKitPath + "Lib\\10.0.17763.0\\ucrt\\x64" + "\""
            // "\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\"",
            // "\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\ucrt\\x64\""
        };

        linkerConfiguration.importLibraries = { "AdvAPI32" };

        return std::make_unique<LinkerImpl>(
            &commandFactory, 
            linkerCommand,
            linkerSwitches, 
            linkerConfiguration
        );
    }
}
