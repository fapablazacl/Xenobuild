
#include <bok/core/toolchain/Linker_Mock.hpp>

#include <boost/optional.hpp>
#include <bok/core/CommandFactory.hpp>

namespace bok {

    static std::string toString(LinkerModuleType type) {
        switch (type) {
        case LinkerModuleType::GuiApplication: return "app-gui";
        case LinkerModuleType::CliApplication: return "app-cli";
        case LinkerModuleType::StaticLibrary: return "lib-static";
        case LinkerModuleType::DynamicLibrary: return "lib-dynamic";
        default: return "<module-unknown>";
        }
    }

    LinkOutput Linker_Mock::generateLinkOutput(const LinkInput& input) const {
        LinkOutput output;

        output.linkCommand.name = "link-c++";
        output.linkCommand.args.push_back("--output=" + input.outputPath.string());
        output.linkCommand.args.push_back("--type=" + toString(input.moduleType));

        std::transform(input.objectFiles.begin(), input.objectFiles.end(), std::back_inserter(output.linkCommand.args), [](const auto &objectFile) {
            return objectFile.string();
        });

        std::transform(input.libraries.begin(), input.libraries.end(), std::back_inserter(output.linkCommand.args), [](const auto &library) {
            return "--library=" + library.string();
        });
        
        std::transform(input.libraryPaths.begin(), input.libraryPaths.end(), std::back_inserter(output.linkCommand.args), [](const auto libraryPath) {
            return "--library-path=" + libraryPath.string();
        });
        
        return output;
    }
}
