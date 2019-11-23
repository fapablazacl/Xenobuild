
#include "LinkerImpl.hpp"

#include <boost/optional.hpp>
#include <borc/model/CommandFactory.hpp>

namespace borc {
    LinkerImpl::LinkerImpl(CommandFactory *commandFactory, const boost::filesystem::path &linkerPath, const std::string &commandName, const LinkerImplSwitches &switches, const std::vector<LinkerImplBuildRule> &buildRules) {
        this->commandFactory = commandFactory;
        this->linkerPath = linkerPath;
        this->commandName = commandName;
        this->switches = switches;
        this->buildRules = buildRules;
    }


    LinkerImpl::~LinkerImpl() {}


    LinkOutput LinkerImpl::link(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles) const {
        boost::optional<LinkerImplBuildRule> buildRule = this->matchBuildRule(module);

        if (! buildRule) {
            throw std::runtime_error("The current toolchain can't build the current module");
        }

        boost::filesystem::path moduleOutputPath = this->buildOuputPath(outputPath, package, module);
        Command *command = this->buildLinkCommand(outputPath, package, module, objectFiles, buildRule.get());

        return { moduleOutputPath, command };
    }


    boost::optional<LinkerImplBuildRule> LinkerImpl::matchBuildRule(const Module *module) const {
        auto matchedBuildRule = std::find_if(buildRules.begin(), buildRules.end(), [module] (const LinkerImplBuildRule &buildRule) {
            const auto &moduleTypes = buildRule.input.moduleTypes;

            auto foundType = std::find_if(moduleTypes.begin(), moduleTypes.end(), [module] (const Module::Type &type) {
                return module->getType() == type;
            });

            return foundType != moduleTypes.end();
        });

        if (matchedBuildRule == buildRules.end()) {
            return {};
        }

        return (*matchedBuildRule);
    }


    Command* LinkerImpl::buildLinkCommand(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles, const LinkerImplBuildRule &buildRule) const {
        std::vector<std::string> commandOptions;

        // options for module output
        const boost::filesystem::path moduleName = module->getName();
        const boost::filesystem::path moduleOutputPath = outputPath / module->getPath() / moduleName;

        commandOptions.push_back(switches.outputFile + moduleOutputPath.string());

        // miscelaneous flags
        commandOptions.insert(commandOptions.end(), buildRule.flags.begin(), buildRule.flags.end());

        // options for module dependencies
        for (const Module *moduleDependency : module->getDependencies()) {
            const std::vector<std::string> options = this->buildModuleDependencyOptions(outputPath, package, module);

            commandOptions.insert(commandOptions.end(), options.begin(), options.end());
        }

        // options for object files
        for (const boost::filesystem::path &objetFile : objectFiles) {
            commandOptions.push_back(objetFile.string());
        }

        // build command
        const boost::filesystem::path commandPath = this->buildCommandPath();
 
        Command *command = commandFactory->createCommand(commandPath.string(), commandOptions);

        return command;
    }


    std::vector<std::string> LinkerImpl::buildModuleDependencyOptions(const boost::filesystem::path &outputPath, const Package *package, const Module *dependency) const {        
        const std::string library = switches.importLibrary + dependency->getName();
        const std::string libraryPath = switches.libraryPath + (outputPath / dependency->getPath()).string();

        return { library, libraryPath };
    }


    boost::filesystem::path LinkerImpl::buildOuputPath(const boost::filesystem::path &outputPath, const Package *package, const Module *module) const {
        const boost::filesystem::path moduleOutputPath = outputPath / module->getPath() / module->getName();

        return moduleOutputPath;
    }


    boost::filesystem::path LinkerImpl::buildCommandPath() const {
        return linkerPath / commandName;
    }
}
