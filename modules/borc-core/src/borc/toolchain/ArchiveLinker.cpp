
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

    LinkOutput ArchiveLinker::link(const boost::filesystem::path &outputPath, const Package *package, const Artifact *artifact, const std::vector<boost::filesystem::path> &objectFiles) const {
        // TODO: Change artifact name based on the current toolchain
        const boost::filesystem::path archiveName = "lib" + artifact->getName() + ".a";
        const boost::filesystem::path archiveOutputPath = outputPath / artifact->getPath() / archiveName;

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
