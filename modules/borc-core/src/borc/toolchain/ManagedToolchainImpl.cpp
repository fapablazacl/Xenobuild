
#include "ManagedToolchainImpl.hpp"

#include <boost/filesystem.hpp>
#include <borc/model/Command.hpp>
#include <borc/entity/ToolchainEntity.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Source.hpp>
#include <borc/toolchain/SourceChecker.hpp>
#include <borc/toolchain/ModuleChecker.hpp>
#include <borc/toolchain/CompilerImpl.hpp>
#include <borc/toolchain/LinkerImpl.hpp>

/* deprecated */
#include <borc/toolchain/ModuleLinker.hpp>
#include <borc/toolchain/ArchiveLinker.hpp>

namespace borc {
    // TODO: Grab them from a service
    static std::initializer_list<std::string> cppSourceFiles = {
        "*.cpp", "*.cxx", "*.c++", "*.cc"
    };

    static std::initializer_list<Module::Type> moduleLinkerModuleTypes ({
        Module::Type{"application", "cli"},
        Module::Type{"application", "gui"},
        Module::Type{"library", "dynamic"}
    });

    static std::initializer_list<Module::Type> archiveLinkerModuleTypes ({
        Module::Type{"library", "static"}
    });


    ManagedToolchainImpl::ManagedToolchainImpl(const ToolchainEntity &entity) {
        for (const ToolchainEntity::Tool &tool : entity.tools) {
            if (tool.type == "compiler") {
                CompilerSwitches compilerSwitches;
                compilerSwitches.compile = tool.switches.compile;
                compilerSwitches.objectFileOutput = tool.switches.outputFile;
                // compilerSwitches.zeroOptimization = "/Od";
                compilerSwitches.includePath = tool.switches.includePath;
                compilerSwitches.includeDebug = tool.switches.debugInformation;

                auto compiler = std::make_unique<CompilerImpl> (
                    &commandFactory, tool.command, compilerSwitches,
                    CompilerConfiguration { 
                        {/*"/EHsc", "/std:c++17"*/}, 
                        {}
                    }
                );

            } else if (tool.type == "linker") {
                LinkerImplSwitches switches;

                switches.importLibrary = tool.switches.importLibrary;
                switches.outputFile = tool.switches.outputFile;
                switches.libraryPath = tool.switches.libraryPath;

                std::vector<LinkerImplBuildRule> buildRules;
                for (const ToolchainEntity::BuildRule &buildRuleEntity : tool.buildRules) {
                    LinkerImplBuildRule buildRule;

                    buildRule = {};
                    buildRule.flags = buildRuleEntity.flags;

                    buildRule.input.fileType = buildRuleEntity.input.fileType;

                    std::transform(
                            buildRuleEntity.input.moduleTypes.begin(),
                            buildRuleEntity.input.moduleTypes.end(), 
                            std::back_inserter(buildRule.input.moduleTypes),
                        [](const std::string &moduleTypeStr) {
                            return *Module::Type::parse(moduleTypeStr);
                    });

                    buildRule.output.fileType = buildRuleEntity.output.fileType;
                    buildRule.output.fileName = buildRuleEntity.output.fileName;

                    buildRules.push_back(buildRule);
                }
                
                linkers.emplace_back(new LinkerImpl{
                    &commandFactory,
                    boost::filesystem::current_path(),
                    tool.command,
                    switches,
                    buildRules
                });
            }
        }
    }


    ManagedToolchainImpl::~ManagedToolchainImpl() {}


    const Compiler* ManagedToolchainImpl::selectCompiler(const Source *source) const {
        for (auto &pair : compilers) {
            if (pair.first->check(source->getFilePath())) {
                return pair.second.get();
            }
        }

        return nullptr;
    }


    const Linker* ManagedToolchainImpl::selectLinker(const Module *module) const {
        for (const auto &linker : linkers) {
            if (linker->isModuleLinkable(module)) {
                return linker.get();
            }
        }

        return nullptr;
    }
}
