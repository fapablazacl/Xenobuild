
#include <borc/toolchain/Linker.hpp>

#include <iostream>
#include <borc/model/Package.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Command.hpp>
#include <borc/model/CommandFactory.hpp>

namespace borc {
	Linker::Linker(CommandFactory *commandFactory, const std::string &commandPath, const LinkerSwitches &switches, const LinkerConfiguration &configuration) {
		this->commandFactory = commandFactory;
		this->commandPath = commandPath;
		this->switches = switches;
		this->configuration = configuration;
	}

	std::string Linker::link(const Package *package, const Artifact *artifact, const std::vector<std::string> &objectFiles) const {
		// std::cout << "Linking ${ArtifactType} module " << artifact->getName() << " ..." << std::endl;

		const std::string outputModuleFilePath = artifact->getPath().string();

		/*
		const auto librariesOptions = this->computeLibrariesOptions(this->collectLibraries(package, artifact));
		const auto libraryPathsOptions = this->computeLibraryPathsOptions(this->collectLibraryPaths(package, artifact));

		Command *command = commandFactory->createCommand(commandPath);

		if (artifact->getType() == Artifact::Type::LibraryDynamic) {
			command->addOption(switches.buildSharedLibrary);
		}

		command->addOptionRange(librariesOptions.begin(), librariesOptions.end());
		command->addOptionRange(libraryPathsOptions.begin(), libraryPathsOptions.end());
		command->addOption(switches.moduleOutput + outputModuleFilePath);
		command->addOptionRange(std::begin(objectFiles), std::end(objectFiles));
		command->execute();
		*/

		return outputModuleFilePath;
	}

	LinkOutput Linker::link(const Package *package, const Artifact *artifact, const std::vector<boost::filesystem::path> &objectFiles) const {
		const std::string outputModuleFilePath = artifact->getPath().string();

		const auto librariesOptions = this->computeLibrariesOptions(this->collectLibraries(package, artifact));
		const auto libraryPathsOptions = this->computeLibraryPathsOptions(this->collectLibraryPaths(package, artifact));

		std::vector<std::string> commandOptions;

		if (artifact->getType() == Artifact::Type::LibraryDynamic) {
			commandOptions.push_back(switches.buildSharedLibrary);
		}

		commandOptions.insert(commandOptions.end(), librariesOptions.begin(), librariesOptions.end());
		commandOptions.insert(commandOptions.end(), libraryPathsOptions.begin(), libraryPathsOptions.end());
		commandOptions.push_back(switches.moduleOutput + outputModuleFilePath);

		for (const boost::filesystem::path &objetFile : objectFiles) {
			commandOptions.push_back(objetFile.string());
		}

		Command *command = commandFactory->createCommand(commandPath, commandOptions);

		return {outputModuleFilePath, command};
	}

	std::vector<std::string> Linker::computeLibrariesOptions(const std::vector<std::string> &libraries) const {
		std::vector<std::string> options;

		for (const std::string &importLibrary : libraries) {
			options.push_back(importLibrary + ".lib");
		}

		return options;
	}

	std::vector<std::string> Linker::computeLibraryPathsOptions(const std::vector<std::string> &paths) const {
		std::vector<std::string> options;

		for (const std::string &path : paths) {
			const std::string option = switches.importLibraryPath + path;
			options.push_back(option);
		}

		return options;
	}

	std::vector<std::string> Linker::collectLibraries(const Package *package, const Artifact *artifact) const {
		std::vector<std::string> libraries = configuration.importLibraries;

		for (const Artifact *dependency : artifact->getDependencies()) {
			const std::string library = dependency->getName();
			libraries.push_back(library);
		}

		return libraries;
	}

	std::vector<std::string> Linker::collectLibraryPaths(const Package *package, const Artifact *artifact) const {
		std::vector<std::string> paths = configuration.importLibraryPaths;

		for (const Artifact *dependency : artifact->getDependencies()) {
			const std::string path = dependency->getPath().string();
			paths.push_back(path);
		}

		return paths;
	}
}
