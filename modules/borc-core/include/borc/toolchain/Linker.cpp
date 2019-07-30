
#include "Linker.hpp"

#include "Command.hpp"
#include "CommandFactory.hpp"
#include "Project.hpp"
#include "Module.hpp"
#include <iostream>

namespace borc {
	Linker::Linker(CommandFactory *commandFactory, const std::string &commandPath, const LinkerSwitches &switches, const LinkerConfiguration &configuration) {
		this->commandFactory = commandFactory;
		this->commandPath = commandPath;
		this->switches = switches;
		this->configuration = configuration;
	}

	std::string Linker::link(const Project *project, const Module *module, const std::vector<std::string> &objectFiles) const {
		std::cout << "Linking " << toString(module->getType()) << " module " << module->getName() << " ..." << std::endl;

		const std::string outputModuleFilePath = module->getOutputFilePath().string();

		const auto librariesOptions = this->computeLibrariesOptions(this->collectLibraries(project, module));
		const auto libraryPathsOptions = this->computeLibraryPathsOptions(this->collectLibraryPaths(project, module));

		Command *command = commandFactory->createCommand(commandPath);

		if (module->getType() == ModuleType::Library) {
			command->addOption(switches.buildSharedLibrary);
		}

		command->addOptionRange(librariesOptions.begin(), librariesOptions.end());
		command->addOptionRange(libraryPathsOptions.begin(), libraryPathsOptions.end());
		command->addOption(switches.moduleOutput + outputModuleFilePath);
		command->addOptionRange(std::begin(objectFiles), std::end(objectFiles));
		command->execute();

		return outputModuleFilePath;
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

	std::vector<std::string> Linker::collectLibraries(const Project *project, const Module *module) const {
		std::vector<std::string> libraries = configuration.importLibraries;

		for (const Module *dependency : module->getDependencies()) {
			const std::string library = dependency->getName();
			libraries.push_back(library);
		}

		return libraries;
	}

	std::vector<std::string> Linker::collectLibraryPaths(const Project *project, const Module *module) const {
		std::vector<std::string> paths = configuration.importLibraryPaths;

		for (const Module *dependency : module->getDependencies()) {
			const std::string path = dependency->getOutputPath().string();
			paths.push_back(path);
		}

		return paths;
	}
}
