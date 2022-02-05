
#include <Xenobuild/core/PackageManager.h>

#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/core/Triplet.h>


namespace Xenobuild {
    PackageManager::PackageManager(CommandExecutor &executor,
                               const std::string& prefixPath,
                               const std::string &toolchainPrefix,
                               const std::string &installSuffix,
                               const unsigned processorCount) :
    executor(executor),
    prefixPath(prefixPath),
    toolchainPrefix(toolchainPrefix),
    installSuffix(installSuffix) {}
    
    bool PackageManager::configure(const Package &package, const Triplet &triplet, const CMakeBuildType buildType) {
        // NOTE: Let's assume that the build system that the current Package uses, is CMake.
        CMakeConfig config;
        
        config.sourcePath = package.path.string();
        config.buildPath = (prefixPath / computePathSuffix(triplet) / evaluate(buildType)).string();
        config.definitions = createConfigDefinitions("", buildType);
        CommandX command = generateCommand(config);
        CommandBatch batch = createCMakeBatch(command, toolchainPrefix);

        const CommandResult result = executor(batch);
        
        if (!result) {
            std::cerr << "Configure command failed." << std::endl;
            write(std::cerr, result.stderr);
            
            return false;
        }
        
        return true;
    }
}
