
#include "ManagedToolchainImpl.hpp"

#include <borc/model/Command.hpp>
#include <borc/entity/ToolchainEntity.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Source.hpp>
#include <borc/toolchain/SourceChecker.hpp>
#include <borc/toolchain/ModuleChecker.hpp>
#include <borc/toolchain/CompilerImpl.hpp>
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
        for (auto &pair : linkers) {
            if (pair.first->check(module)) {
                return pair.second.get();
            }
        }

        return nullptr;
    }
}
