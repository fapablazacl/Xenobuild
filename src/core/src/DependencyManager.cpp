
#include <Xenobuild/core/DependencyManager.h>
#include <Xenobuild/core/URL.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Dependency.h>
#include <boost/algorithm/string.hpp>


namespace Xenobuild {
    DependencyManager::DependencyManager(CommandExecutor &executor,
                               const std::string& prefixPath,
                               const std::string &toolchainPrefix,
                               const std::string &installSuffix,
                               const unsigned processorCount) :
    executor(executor),
    prefixPath(prefixPath),
    toolchainPrefix(toolchainPrefix),
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
    

    bool DependencyManager::configure(const Dependency& dependency, const CMakeBuildType buildType, const std::string &generator) {
        const URL url = URL::parse(dependency.url);
        
        const auto sourcePath = computePath(prefixPath / "sources", url, dependency.tag);
        
        const auto buildPath = computePath(sourcePath, buildType);
        const auto installPath = computePath(prefixPath / "packages" / installSuffix, url, dependency.version);

        CMakeConfig config {
            sourcePath.string(),
            buildPath.string(),
            generator,
            createConfigDefinitions(installPath, buildType)
        };

        config.definitions.insert(dependency.definitions.begin(), dependency.definitions.end());

        CommandX command = generateCommand(config);
        CommandBatch batch = createCMakeBatch(command, toolchainPrefix);

        const CommandResult result = executor(batch);
        
        if (!result) {
            std::cerr << "Configure command failed." << std::endl;
            write(std::cerr, result.err);
            
            return false;
        }

        return true;
    }

    
    bool DependencyManager::build(const Dependency& dependency, const CMakeBuildType buildType) {
        const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url), dependency.tag);
        const auto buildPath = computePath(sourcePath, buildType);
        
        // FIX: Using parallel build with CMake, causes deadlock in glfw.
        CMakeBuild build { buildPath.string()/*, processorCount */};
        CommandX command = generateCommand(build);
        CommandBatch batch = createCMakeBatch(command, toolchainPrefix);

        const CommandResult result = executor(batch);
        
        if (!result) {
            std::cerr << "Build command failed." << std::endl;
            write(std::cerr, result.err);
            
            return false;
        }

        return true;
    }

    
    bool DependencyManager::install(const Dependency& dependency, const CMakeBuildType buildType) {
        const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url), dependency.tag);
        const auto buildPath = computePath(sourcePath, buildType);
        
        CMakeInstall install { buildPath.string() };
        CommandX command = generateCommand(install);
        CommandBatch batch = createCMakeBatch(command, toolchainPrefix);

        const CommandResult result = executor(batch);
        
        if (!result) {
            std::cerr << "Install command failed." << std::endl;
            write(std::cerr, result.err);
            
            return false;
        }

        return true;
    }

    
    boost::filesystem::path DependencyManager::computeInstallPath(const Dependency &dependency, const CMakeBuildType buildType) const {
     
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
