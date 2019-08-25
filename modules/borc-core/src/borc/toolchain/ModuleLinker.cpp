
#include "ModuleLinker.hpp"

#include <iostream>
#include <borc/model/Package.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Command.hpp>
#include <borc/model/CommandFactory.hpp>

namespace borc {
	ModuleLinker::ModuleLinker(CommandFactory *commandFactory, const std::string &commandPath, const LinkerSwitches &switches, const LinkerConfiguration &configuration) {
		this->commandFactory = commandFactory;
		this->commandPath = commandPath;
		this->switches = switches;
		this->configuration = configuration;
	}

    ModuleLinker::~ModuleLinker() {}

	LinkOutput ModuleLinker::link(const boost::filesystem::path &outputPath, const Package *package, const Artifact *artifact, const std::vector<boost::filesystem::path> &objectFiles) const {
		// TODO: Change artifact name based on the current toolchain
        boost::filesystem::path moduleName = artifact->getName();

		if (artifact->getType() == Artifact::Type::LibraryDynamic) {
			moduleName = "lib" + moduleName.string() + ".so";
		}

        const boost::filesystem::path moduleOutputPath = outputPath / artifact->getPath() / moduleName;

		const auto librariesOptions = this->computeLibrariesOptions(this->collectLibraries(package, artifact));
		const auto libraryPathsOptions = this->computeLibraryPathsOptions(this->collectLibraryPaths(package, artifact, outputPath));

		std::vector<std::string> commandOptions;

		if (artifact->getType() == Artifact::Type::LibraryDynamic) {
			commandOptions.push_back(switches.buildSharedLibrary);
		}

		commandOptions.insert(commandOptions.end(), librariesOptions.begin(), librariesOptions.end());
		commandOptions.insert(commandOptions.end(), libraryPathsOptions.begin(), libraryPathsOptions.end());
		commandOptions.push_back(switches.moduleOutput + moduleOutputPath.string());

		for (const boost::filesystem::path &objetFile : objectFiles) {
			commandOptions.push_back(objetFile.string());
		}

		Command *command = commandFactory->createCommand(commandPath, commandOptions);

		return {moduleOutputPath, command};
	}

	std::vector<std::string> ModuleLinker::computeLibrariesOptions(const std::vector<std::string> &libraries) const {
		std::vector<std::string> options;

		for (const std::string &importLibrary : libraries) {
			// options.push_back(importLibrary + ".lib");
			options.push_back("-l" + importLibrary);
		}

		return options;
	}

	std::vector<std::string> ModuleLinker::computeLibraryPathsOptions(const std::vector<std::string> &paths) const {
		std::vector<std::string> options;

		for (const std::string &path : paths) {
			const std::string option = switches.importLibraryPath + path;
			options.push_back(option);
		}

		return options;
	}

	std::vector<std::string> ModuleLinker::collectLibraries(const Package *package, const Artifact *artifact) const {
		std::vector<std::string> libraries = configuration.importLibraries;

		for (const Artifact *dependency : artifact->getDependencies()) {
			const std::string library = dependency->getName();
			libraries.push_back(library);
		}

		return libraries;
	}

	std::vector<std::string> ModuleLinker::collectLibraryPaths(const Package *package, const Artifact *artifact, const boost::filesystem::path &outputPath) const {
		std::vector<std::string> paths = configuration.importLibraryPaths;

		for (const Artifact *dependency : artifact->getDependencies()) {
			const std::string path = (outputPath / dependency->getPath()).string();
			paths.push_back(path);
		}

		return paths;
	}
}
