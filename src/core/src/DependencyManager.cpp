
#include <Xenobuild/core/DependencyManager.h>

#include <boost/algorithm/string.hpp>
#include <Xenobuild/core/URL.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Toolchain.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/core/CMakeBuildSystem.h>
#include <Xenobuild/core/GitRepository.h>


namespace Xenobuild {
    DependencyManager::DependencyManager(CommandExecutor &executor,
                               const std::string& prefixPath,
                               const std::string &installSuffix) :
    executor(executor),
    prefixPath(prefixPath),
    installSuffix(installSuffix) {}

    
    bool DependencyManager::download(const Dependency& dependency) const {
        const URL url = URL::parse(dependency.url);
        const auto repository = GitRepository { dependency.url, dependency.tag };
        const auto sourcePath = computePath(prefixPath / "sources", url, dependency.tag);

        const CommandResult result = repository.clone(executor, sourcePath);

        if (!result) {
            std::cerr << "Repository failed." << std::endl;
            write(std::cerr, result.err);
            
            return false;
        }
        
        return true;
    }
    

    bool DependencyManager::configure(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType, const boost::optional<CMakeGenerator> generator) {
        const URL url = URL::parse(dependency.url);
        
        const auto sourcePath = computePath(prefixPath / "sources", url, dependency.tag);
        
        const auto buildPath = computePath(sourcePath, buildType);
        const auto installPath = computePath(prefixPath / "packages" / installSuffix, url, dependency.version);

        CMakeConfiguration config {
            sourcePath.string(),
            buildPath.string(),
            generator,
            createConfigDefinitions(installPath, buildType)
        };

        config.definitions.insert(dependency.definitions.begin(), dependency.definitions.end());

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

    
    bool DependencyManager::build(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType) {
        const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url), dependency.tag);
        const auto buildPath = computePath(sourcePath, buildType);
        
        // FIX: Using parallel build with CMake, causes deadlock in glfw.
        CMakeBuild build { buildPath.string()/*, processorCount */};

        CommandBatch batch{ generateCommand(build) };

        if (auto toolchainCommand = toolchain.createEnvCommand(); toolchainCommand) {
            batch.commands.push_back(toolchainCommand.get());
        }

        const CommandResult result = executor(batch);
        
        if (!result) {
            std::cerr << "Build command failed." << std::endl;
            write(std::cerr, result.err);
            
            return false;
        }

        return true;
    }

    
    bool DependencyManager::install(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType) {
        const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url), dependency.tag);
        const auto buildPath = computePath(sourcePath, buildType);
        
        CMakeInstall install { buildPath.string() };

        CommandBatch batch{ generateCommand(install) };

        if (auto toolchainCommand = toolchain.createEnvCommand(); toolchainCommand) {
            batch.commands.push_back(toolchainCommand.get());
        }

        const CommandResult result = executor(batch);
        
        if (!result) {
            std::cerr << "Install command failed." << std::endl;
            write(std::cerr, result.err);
            
            return false;
        }

        return true;
    }

    
    boost::filesystem::path DependencyManager::computeInstallPath(const Dependency &dependency) const {
        const URL url = URL::parse(dependency.url);
        const boost::filesystem::path installPath = computePath(prefixPath / "packages" / installSuffix, url, dependency.version);
        
        return installPath;
    }
    
    
    boost::filesystem::path DependencyManager::computePath(const boost::filesystem::path& prefix, const CMakeBuildType type) const {
        switch (type) {
        case CMakeBuildType::Default:
            return prefix;

        case CMakeBuildType::Debug:
            return prefix / "debug";

        case CMakeBuildType::Release:
            return prefix / "release";
        }

        return prefix;
    }


    boost::filesystem::path DependencyManager::computePath(const boost::filesystem::path& prefix, const URL url, const std::string &suffix) const {
        boost::filesystem::path sourcePath{ prefix / url.host };

        std::vector<std::string> pathParts;
        boost::split(pathParts, url.path, boost::is_any_of("/"));

        for (const auto& part : pathParts) {
            sourcePath /= part;
        }

        sourcePath /= suffix;

        return sourcePath;
    }
}
