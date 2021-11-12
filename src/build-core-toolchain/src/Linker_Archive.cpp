
#include <bok/core/toolchain/Linker_Archive.hpp>

#include <iostream>
#include <bok/core/Command.hpp>
#include <bok/core/CommandFactory.hpp>

namespace bok {
    Linker_Archive::Linker_Archive(CommandFactory *commandFactory, const std::string &commandPath, const Linker_Archive::Switches &switches) {
        this->commandFactory = commandFactory;
        this->commandPath = commandPath;
        this->switches = switches;
    }

    Linker_Archive::~Linker_Archive() {}

    LinkOutput Linker_Archive::link(const boost::filesystem::path &outputPath, const Package *package, const Module *component, const std::vector<boost::filesystem::path> &objectFiles) const {
        // TODO: Change component name based on the current toolchain
        const boost::filesystem::path archiveName = "lib" + /*component->getName() +*/ std::string(".a");
        const boost::filesystem::path archiveOutputPath = outputPath / /*component->getPath() / */archiveName;

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
