
#include <bok/core/toolchain/Linker_Module.hpp>

#include <iostream>
#include <bok/core/Command.hpp>
#include <bok/core/CommandFactory.hpp>

namespace bok {
    Linker_Module::Linker_Module(CommandFactory *commandFactory, const std::string &commandPath, const LinkerSwitches &switches, const LinkerConfiguration &configuration) {
        this->commandFactory = commandFactory;
        this->commandPath = commandPath;
        this->switches = switches;
        this->configuration = configuration;
    }

    Linker_Module::~Linker_Module() {}

    LinkOutput Linker_Module::link(const boost::filesystem::path &outputPath, const Package *package, const Module *component, const std::vector<boost::filesystem::path> &objectFiles) const {
        // TODO: Change component name based on the current toolchain
        boost::filesystem::path moduleName /* = component->getName()*/;

        /*
        if (component->getType() == Module::Type{"library", "dynamic"}) {
            moduleName = "lib" + moduleName.string() + ".so";
        }
        */

        const boost::filesystem::path moduleOutputPath = outputPath / /*component->getPath() / */ moduleName;

        const auto librariesOptions = this->computeLibrariesOptions(this->collectLibraries(package, component));
        const auto libraryPathsOptions = this->computeLibraryPathsOptions(this->collectLibraryPaths(package, component, outputPath));

        std::vector<std::string> commandOptions;

        /*if (component->getType() == Module::Type{"library", "dynamic"}) {
            commandOptions.push_back(switches.buildSharedLibrary);
        }
        */

        commandOptions.insert(commandOptions.end(), librariesOptions.begin(), librariesOptions.end());
        commandOptions.insert(commandOptions.end(), libraryPathsOptions.begin(), libraryPathsOptions.end());
        commandOptions.push_back(switches.moduleOutput + moduleOutputPath.string());

        for (const boost::filesystem::path &objetFile : objectFiles) {
            commandOptions.push_back(objetFile.string());
        }

        Command *command = commandFactory->createCommand(commandPath, commandOptions);

        //return {moduleOutputPath, command};
        return {};
    }

    std::vector<std::string> Linker_Module::computeLibrariesOptions(const std::vector<std::string> &libraries) const {
        std::vector<std::string> options;

        for (const std::string &importLibrary : libraries) {
            // options.push_back(importLibrary + ".lib");
            options.push_back("-l" + importLibrary);
        }

        return options;
    }

    std::vector<std::string> Linker_Module::computeLibraryPathsOptions(const std::vector<std::string> &paths) const {
        std::vector<std::string> options;

        for (const std::string &path : paths) {
            const std::string option = switches.importLibraryPath + path;
            options.push_back(option);
        }

        return options;
    }

    std::vector<std::string> Linker_Module::collectLibraries(const Package *package, const Module *component) const {
        std::vector<std::string> libraries = configuration.importLibraries;

        /*
        for (const Module *dependency : component->getDependencies()) {
            const std::string library = dependency->getName();
            libraries.push_back(library);
        }
        */

        return libraries;
    }

    std::vector<std::string> Linker_Module::collectLibraryPaths(const Package *package, const Module *component, const boost::filesystem::path &outputPath) const {
        std::vector<std::string> paths = configuration.importLibraryPaths;

        /*
        for (const Module *dependency : component->getDependencies()) {
            const std::string path = (outputPath / dependency->getPath()).string();
            paths.push_back(path);
        }
        */

        return paths;
    }
}
