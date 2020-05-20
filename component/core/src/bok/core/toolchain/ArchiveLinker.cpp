
#include "ArchiveLinker.hpp"

#include <iostream>
#include <bok/model/Package.hpp>
#include <bok/model/Module.hpp>
#include <bok/model/Command.hpp>
#include <bok/model/CommandFactory.hpp>

namespace bok {
    ArchiveLinker::ArchiveLinker(CommandFactory *commandFactory, const std::string &commandPath, const ArchiveLinker::Switches &switches) {
        this->commandFactory = commandFactory;
        this->commandPath = commandPath;
        this->switches = switches;
    }

    ArchiveLinker::~ArchiveLinker() {}

    LinkOutput ArchiveLinker::link(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles) const {
        // TODO: Change module name based on the current toolchain
        const boost::filesystem::path archiveName = "lib" + module->getName() + ".a";
        const boost::filesystem::path archiveOutputPath = outputPath / module->getPath() / archiveName;

        std::vector<std::string> commandOptions;
        commandOptions.push_back(switches.buildStaticLibrary);
        commandOptions.push_back(switches.moduleOutput + archiveOutputPath.string());

        for (const boost::filesystem::path &objetFile : objectFiles) {
            commandOptions.push_back(objetFile.string());
        }

        Command *command = commandFactory->createCommand(commandPath, commandOptions);

        return {archiveOutputPath, command};
    }
}
