
#include "LinkerImpl.hpp"

#include <boost/optional.hpp>
#include <bok/model/CommandFactory.hpp>

namespace bok {
    struct LinkerImpl::Private {
    public:
		CommandFactory *commandFactory = nullptr;
        boost::filesystem::path linkerPath; 
        std::string commandName; 
        LinkerImpl::Switches switches;
        std::vector<LinkerImpl::BuildRule> buildRules;

	public:
        boost::optional<LinkerImpl::BuildRule> matchBuildRule(const Module *module) const {
            auto matchedBuildRule = std::find_if(buildRules.begin(), buildRules.end(), [module] (const LinkerImpl::BuildRule &buildRule) {
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


        Command* buildLinkCommand(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles, const LinkerImpl::BuildRule &buildRule) const {
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


        std::vector<std::string> buildModuleDependencyOptions(const boost::filesystem::path &outputPath, const Package *package, const Module *dependency) const {        
            const std::string library = switches.importLibrary + dependency->getName();
            const std::string libraryPath = switches.libraryPath + (outputPath / dependency->getPath()).string();

            return { library, libraryPath };
        }


        boost::filesystem::path buildOuputPath(const boost::filesystem::path &outputPath, const Package *package, const Module *module) const {
            const boost::filesystem::path moduleOutputPath = outputPath / module->getPath() / module->getName();

            return moduleOutputPath;
        }


        boost::filesystem::path buildCommandPath() const {
            return linkerPath / commandName;
        }
    };


    LinkerImpl::LinkerImpl(CommandFactory *commandFactory, const boost::filesystem::path &linkerPath, const std::string &commandName, const LinkerImpl::Switches &switches, const std::vector<LinkerImpl::BuildRule> &buildRules) : m_impl(new LinkerImpl::Private{}) {
        m_impl->commandFactory = commandFactory;
        m_impl->linkerPath = linkerPath;
        m_impl->commandName = commandName;
        m_impl->switches = switches;
        m_impl->buildRules = buildRules;
    }


    LinkerImpl::~LinkerImpl() {
        delete m_impl;
    }


    bool LinkerImpl::isModuleLinkable(const Module *module) const {
        if (auto buildRule = m_impl->matchBuildRule(module); buildRule) {
            return true;
        }

        return false;
    }


    LinkOutput LinkerImpl::link(const boost::filesystem::path &outputPath, const Package *package, const Module *module, const std::vector<boost::filesystem::path> &objectFiles) const {
        boost::optional<LinkerImpl::BuildRule> buildRule = m_impl->matchBuildRule(module);

        if (! buildRule) {
            throw std::runtime_error("The current toolchain can't build the current module");
        }

        boost::filesystem::path moduleOutputPath = m_impl->buildOuputPath(outputPath, package, module);
        Command *command = m_impl->buildLinkCommand(outputPath, package, module, objectFiles, buildRule.get());

        return { moduleOutputPath, command };
    }
}
