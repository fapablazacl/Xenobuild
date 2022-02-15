
#include <Xenobuild/ConfigureController.h>
#include <Xenobuild/Common.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Version.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/core/DependencyManager.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageManager.h>
#include <Xenobuild/core/CMakeBuildSystem.h>


namespace Xenobuild {
    const boost::filesystem::path currentPath = CMAKE_CURRENT_SOURCE_DIR;
    
    ConfigureControllerInput ConfigureControllerInput::parse(int, char**) {
        ConfigureControllerInput result;
        
        result.sourceDir = currentPath.string();
        result.buildDir = (currentPath / ".Xenobuild").string();
        
        return result;
    }
}


namespace Xenobuild {
    const char* ConfigureController::Name = "configure";
    
    ConfigureController::ConfigureController(Package &package, const ConfigureControllerInput& params)
        : package(package), params(params) {}

    void ConfigureController::perform() {
        // For CMake projects, generates all the build configurations required to build the main package.
        
        std::cout << "ConfigureController::perform" << std::endl;

        const unsigned processorCount = 1;
        
        // show current execution environment
        if (params.showEnvironment) {
            std::cout << "USER: \"" << std::getenv("USER") << "\"" << std::endl;
            std::cout << "PATH: \"" << std::getenv("PATH") << "\"" << std::endl;
            std::cout << "SHELL: \"" << std::getenv("SHELL") << "\"" << std::endl;
            std::cout << "Detected CPU Cores: \"" << processorCount << "\"" << std::endl;
        }
        
        // Pick a Default Toolchain, for Windows
        // for other platforms, use the default toolchain
        std::string toolchainPrefix;

        // TODO: Refactor Visual C++ installation pathdetection
        //if (detectHostOS() == OS::Windows) {
        //    const std::vector<std::string> toolchainPrefixPaths = enumerateVCInstallations();
        // 
        //    if (toolchainPrefixPaths.size() > 0) {
        //        toolchainPrefix = toolchainPrefixPaths[0];
        //    }
        //}
        
        // By default, use the local user path to store package repositories
        const boost::filesystem::path prefix = package.path;
        const boost::filesystem::path userPath = getUserPath();
        const std::string suffix = computePathSuffix(params.triplet);

        // BoostProcessCommandExecutor executor;
        SystemCommandExecutor executor;
        PackageManager manager {
            executor,
            (prefix / ".Xenobuild").string(),
            toolchainPrefix,
            suffix,
            processorCount
        };
        
        DependencyManager dependencyManager {
            executor,
            (userPath / ".Xenobuild").string(),
            toolchainPrefix,
            suffix,
            processorCount
        };
        
        const CMakeBuildType buildTypes[] = {
            CMakeBuildType::Release, CMakeBuildType::Debug
        };
        
        for (const CMakeBuildType buildType : buildTypes) {
            if (! manager.configure(package, params.triplet, buildType, dependencyManager)) {
                throw std::runtime_error("Configure command failed.");
            }
        }
    }
}
