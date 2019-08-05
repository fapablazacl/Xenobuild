
#include "LinkerImpl.hpp"

#include <iostream>
#include <borc/model/Package.hpp>
#include <borc/model/Artifact.hpp>
#include <borc/model/Command.hpp>
#include <borc/model/CommandFactory.hpp>

namespace borc {
	LinkerImpl::LinkerImpl(CommandFactory *commandFactory, const std::string &commandPath, const LinkerSwitches &switches, const LinkerConfiguration &configuration) {
		this->commandFactory = commandFactory;
		this->commandPath = commandPath;
		this->switches = switches;
		this->configuration = configuration;
	}

    LinkerImpl::~LinkerImpl() {}

	LinkOutput LinkerImpl::link(const boost::filesystem::path &outputPath, const Package *package, const Artifact *artifact, const std::vector<boost::filesystem::path> &objectFiles) const {
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

	std::vector<std::string> LinkerImpl::computeLibrariesOptions(const std::vector<std::string> &libraries) const {
		std::vector<std::string> options;

		for (const std::string &importLibrary : libraries) {
			// options.push_back(importLibrary + ".lib");
			options.push_back("-l" + importLibrary);
		}

		return options;
	}

	std::vector<std::string> LinkerImpl::computeLibraryPathsOptions(const std::vector<std::string> &paths) const {
		std::vector<std::string> options;

		for (const std::string &path : paths) {
			const std::string option = switches.importLibraryPath + path;
			options.push_back(option);
		}

		return options;
	}

	std::vector<std::string> LinkerImpl::collectLibraries(const Package *package, const Artifact *artifact) const {
		std::vector<std::string> libraries = configuration.importLibraries;

		for (const Artifact *dependency : artifact->getDependencies()) {
			const std::string library = dependency->getName();
			libraries.push_back(library);
		}

		return libraries;
	}

	std::vector<std::string> LinkerImpl::collectLibraryPaths(const Package *package, const Artifact *artifact) const {
		std::vector<std::string> paths = configuration.importLibraryPaths;

		for (const Artifact *dependency : artifact->getDependencies()) {
			const std::string path = dependency->getPath().string();
			paths.push_back(path);
		}

		return paths;
	}
}
