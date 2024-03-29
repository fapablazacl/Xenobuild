
#include <Xenobuild/core/PackageManager.h>

#include <boost/algorithm/string.hpp>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Toolchain.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/core/DependencyManager.h>
#include <Xenobuild/core/Triplet.h>
#include <Xenobuild/core/CMakeBuildSystem.h>


namespace Xenobuild {
    PackageManager::PackageManager(CommandExecutor &executor,
                               const std::string& prefixPath,
                               const std::string &installSuffix) :
    executor(executor),
    prefixPath(prefixPath),
    installSuffix(installSuffix) {}
    
    bool PackageManager::configure(const Package &package, const Toolchain &toolchain, const Triplet &triplet, const CMakeBuildType buildType, const DependencyManager &dependencyManager) {
        // NOTE: Let's assume that the build system that the current Package uses, is CMake.
        CMakeConfiguration config;

        config.sourcePath = package.path.string();
        config.buildPath = (prefixPath / computePathSuffix(triplet) / evaluate(buildType)).string();
        config.definitions = createConfigDefinitions("", buildType);
        
        // manage dependency install locations
        for (const Dependency &dependency : package.dependencies) {
            const std::map<std::string, std::string> variableMap {
                {"${installPrefix}", dependencyManager.computeInstallPath(dependency).string()}
            };
            
            for (const auto &definitionValuePair : dependency.publicdefs) {
                std::string const &definition = definitionValuePair.first;
                std::string value = definitionValuePair.second;
                
                for (const auto &variableValuePair : variableMap) {
                    boost::replace_all(value, variableValuePair.first, variableValuePair.first);
                }
                
                // append now the list of definitions
                config.definitions.insert({definition, value});
            }
        }
        
        CommandBatch batch{ generateCommand(config) };

        if (auto toolchainCommand = toolchain.createEnvCommand(); toolchainCommand) {
            batch.commands.push_back(toolchainCommand.get());
        }

        const CommandResult result = executor(batch);
        
        if (!result) {
            std::cerr << "Configure command failed." << std::endl;
            write(std::cerr, result.err);
            
            return false;
        }
        
        return true;
    }
}
