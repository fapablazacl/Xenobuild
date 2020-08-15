
#include <bok/core/toolchain/vc/Toolchain_VC.hpp>

#include <bok/core/Version.hpp>

namespace bok {
    Toolchain_VC::Toolchain_VC(std::optional<boost::filesystem::path> installationPath) : installationPath(installationPath) {
        // TODO: This depends on the host/target architecture
        const std::string postfix = computePostfix();

        if (installationPath) {
            compilers.emplace_back(new Compiler_VC{ Compiler_VC_Path{*installationPath, postfix} });

            linkers.emplace_back(new Linker_VC{*installationPath / postfix});
        } else {
            compilers.emplace_back(new Compiler_VC{ {} });
            linkers.emplace_back(new Linker_VC{ {} });
        }
    }

    Toolchain_VC::~Toolchain_VC() {}

    std::vector<Compiler*> Toolchain_VC::enumerateCompilers() const {
        return { compilers[0].get() };
    }

    std::vector<Linker*> Toolchain_VC::enumerateLinkers() const {
        return { linkers[0].get() };
    }

    Version Toolchain_VC::detectVersion() const {
        // TODO: Compute compiler version
        return {1, 0, 0};
    }

    Compiler* Toolchain_VC::getCompiler(const CompilerType type) const {
        if (type == CompilerType::Source) {
            return compilers[0].get();
        }

        return nullptr;
    }

    Linker* Toolchain_VC::getLinker(const LinkerType type) const {
        if (type == LinkerType::Binary) {
            return linkers[0].get();
        }

        return nullptr;
    }

    std::string Toolchain_VC::computePostfix() const {
        return "bin/Hostx64/x64";
    }

    //    const std::string commandBasePath = installationPath + "bin\\Hostx64\\x64\\";
    //    const std::string compilerCommand = commandBasePath + "cl.exe";
    //    const std::string linkerCommand = commandBasePath + "link.exe";
    //        std::unique_ptr<Linker> ServiceFactoryVC::createLinker(const std::string &linkerCommand, const std::string &installationPath, const std::string &windowsKitPath) {
    //    LinkerSwitches linkerSwitches;
    //    linkerSwitches.buildSharedLibrary = "/DLL";
    //    linkerSwitches.moduleOutput = "/OUT:";
    //    linkerSwitches.importLibrary = "/IMPLIB:";
    //    linkerSwitches.importLibraryPath = "/LIBPATH:";
    //    LinkerConfiguration linkerConfiguration;
    //    linkerConfiguration.importLibraryPaths = {
    //        "\"" + installationPath + "lib\\x64" + "\"",
    //        "\"" + windowsKitPath + "Lib\\10.0.17763.0\\um\\x64" + "\"",
    //        "\"" + windowsKitPath + "Lib\\10.0.17763.0\\ucrt\\x64" + "\""
    //        // "\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\"",
    //        // "\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\ucrt\\x64\""
    //    };
    //    linkerConfiguration.importLibraries = { "AdvAPI32" };
    //    return std::make_unique<ModuleLinker>(
    //        &commandFactory, 
    //        linkerCommand,
    //        linkerSwitches, 
    //        linkerConfiguration
    //    );
    //}
}
