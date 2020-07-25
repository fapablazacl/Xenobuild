
#include <bok/core/toolchain/ArchiveLinker.hpp>

#include <iostream>
#include <bok/core/Command.hpp>
#include <bok/core/CommandFactory.hpp>
#include <bok/core/package/Package.hpp>
#include <bok/core/package/Module.hpp>

namespace bok {
    ArchiveLinker::ArchiveLinker(CommandFactory *commandFactory, const std::string &commandPath, const ArchiveLinker::Switches &switches) {
        this->commandFactory = commandFactory;
        this->commandPath = commandPath;
        this->switches = switches;
    }

    ArchiveLinker::~ArchiveLinker() {}

    LinkOutput ArchiveLinker::link(const boost::filesystem::path &outputPath, const Package *package, const Component *component, const std::vector<boost::filesystem::path> &objectFiles) const {
        // TODO: Change component name based on the current toolchain
        const boost::filesystem::path archiveName = "lib" + component->getName() + ".a";
        const boost::filesystem::path archiveOutputPath = outputPath / component->getPath() / archiveName;

        std::vector<std::string> commandOptions;
        commandOptions.push_back(switches.buildStaticLibrary);
        commandOptions.push_back(switches.moduleOutput + archiveOutputPath.string());

        for (const boost::filesystem::path &objetFile : objectFiles) {
            commandOptions.push_back(objetFile.string());
        }

        Command *command = commandFactory->createCommand(commandPath, commandOptions);

        // return {archiveOutputPath, command};

        return {};
    }
}
