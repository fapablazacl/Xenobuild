
#include <bok/core/toolchain/LinkerMock.hpp>

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

    LinkOutput LinkerMock::generateLinkOutput(const LinkInput& input) {
        LinkOutput output;

        output.linkCommand.name = "link-c++";
        output.linkCommand.args.push_back("--output=" + input.outputPath);
        output.linkCommand.args.push_back("--type=" + toString(input.moduleType));

        std::copy(input.objectFiles.begin(), input.objectFiles.end(), std::back_inserter(output.linkCommand.args));
        std::transform(input.libraries.begin(), input.libraries.end(), std::back_inserter(output.linkCommand.args), [](const std::string &library) {
            return "--library=" + library;
        });
        
        std::transform(input.libraryPaths.begin(), input.libraryPaths.end(), std::back_inserter(output.linkCommand.args), [](const std::string& libraryPath) {
            return "--library-path=" + libraryPath;
        });
        
        return output;
    }
}
