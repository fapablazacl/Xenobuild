
#include <bok/core/toolchain/vc/Linker_VC.hpp>
#include <optional>

namespace bok {
    static std::optional<std::string> getCompilerOption(LinkerModuleType type) {
        switch (type) {
        case LinkerModuleType::GuiApplication: return "/SUBSYSTEM:WINDOWS";
        case LinkerModuleType::CliApplication: return "/SUBSYSTEM:CONSOLE";
        case LinkerModuleType::DynamicLibrary: return "/DLL";
        default: return {};
        }
    }
    
    Linker_VC::Linker_VC(std::optional<std::string> path) : path(path) {}

    LinkOutput Linker_VC::generateLinkOutput(const LinkInput& input) const {
        LinkOutput output;

        if (path) {
            output.linkCommand.path = *path;
        }

        output.linkCommand.name = "link.exe";
        output.linkCommand.args.push_back("/nologo");
        output.linkCommand.args.push_back("/OUT:" + input.outputPath + "\\" + input.moduleName + ".exe");

        if (auto option = getCompilerOption(input.moduleType); option) {
            output.linkCommand.args.push_back(*option);
        }

        std::copy(input.objectFiles.begin(), input.objectFiles.end(), std::back_inserter(output.linkCommand.args));
        std::transform(input.libraries.begin(), input.libraries.end(), std::back_inserter(output.linkCommand.args), [](const std::string &library) {
            return library;
        });
        
        std::transform(input.libraryPaths.begin(), input.libraryPaths.end(), std::back_inserter(output.linkCommand.args), [](const std::string& libraryPath) {
            return "/LIBPATH:" + libraryPath;
        });
        
        return output;

        //LinkerSwitches linkerSwitches;
        //linkerSwitches.buildSharedLibrary = "/DLL";
        //linkerSwitches.moduleOutput = "/OUT:";
        //linkerSwitches.importLibrary = "/IMPLIB:";
        //linkerSwitches.importLibraryPath = "/LIBPATH:";
        //LinkerConfiguration linkerConfiguration;
        //linkerConfiguration.importLibraryPaths = {
        //    "\"" + installationPath + "lib\\x64" + "\"",
        //    "\"" + windowsKitPath + "Lib\\10.0.17763.0\\um\\x64" + "\"",
        //    "\"" + windowsKitPath + "Lib\\10.0.17763.0\\ucrt\\x64" + "\""
        //    // "\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\"",
        //    // "\"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\ucrt\\x64\""
        //};
        //linkerConfiguration.importLibraries = { "AdvAPI32" };
        

        //ServiceFactoryVC::ServiceFactoryVC(const std::string &installationPath, const std::string &windowsKitPath) {
        //    const std::string commandBasePath = installationPath + "bin\\Hostx64\\x64\\";
        //    const std::string linkerCommand = commandBasePath + "link.exe";
        //}
    }
}
