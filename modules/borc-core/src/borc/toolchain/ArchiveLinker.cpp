
#include "ArchiveLinker.hpp"

#include <iostream>
#include <borc/model/Package.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Command.hpp>
#include <borc/model/CommandFactory.hpp>

namespace borc {
    ArchiveLinker::ArchiveLinker(CommandFactory *commandFactory, const std::string &commandPath, const ArchiveLinker::Switches &switches) {
        this->commandFactory = commandFactory;
        this->commandPath = commandPath;
        this->switches = switches;
    }


    ArchiveLinker::~ArchiveLinker() {}


    LinkOutput ArchiveLinker::link(const Package *package, const Artifact *artifact, const std::vector<boost::filesystem::path> &objectFiles) const {
        const std::string outputModuleFilePath = artifact->getPath().string();

		std::vector<std::string> commandOptions;
        commandOptions.push_back(switches.buildStaticLibrary);
        commandOptions.push_back(switches.moduleOutput + outputModuleFilePath);

        for (const boost::filesystem::path &objetFile : objectFiles) {
			commandOptions.push_back(objetFile.string());
		}

		Command *command = commandFactory->createCommand(commandPath, commandOptions);

		return {outputModuleFilePath, command};
    }
}
