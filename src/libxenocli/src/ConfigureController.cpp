
#include <Xenobuild/ConfigureController.h>

#include <boost/optional/optional_io.hpp>
#include <Xenobuild/Common.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Context.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/core/DependencyManager.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageManager.h>
#include <Xenobuild/core/CMakeBuildSystem.h>


namespace Xenobuild {
    const boost::filesystem::path currentPath = CMAKE_CURRENT_SOURCE_DIR;
    
    ConfigureControllerInput ConfigureControllerInput::parse(const std::vector<std::string> &) {
        ConfigureControllerInput result;
        
        result.sourceDir = currentPath.string();
        result.buildDir = (currentPath / ".Xenobuild").string();
        
        return result;
    }
}


namespace Xenobuild {
    const char* ConfigureController::Name = "configure";
    
    ConfigureController::ConfigureController(
        Context &context, 
        const ConfigureControllerInput& params) : context(context), params(params) {}


    ConfigureController::ConfigureController(
        Context &context, 
        const ConfigureControllerInput &params, 
        std::unique_ptr<CommandExecutor> executor) : context(context), params(params), executor(std::move(executor)) {}


    ConfigureController::~ConfigureController() {}


    void ConfigureController::perform() {
        // For CMake projects, generates all the build configurations required to build the main package.
        std::cout << "ConfigureController::perform" << std::endl;

        const unsigned processorCount = 1;
        
        // show current execution environment
        if (params.showEnvironment) {
            std::cout << "USER: \"" << getenv("USER") << "\"" << std::endl;
            std::cout << "PATH: \"" << getenv("PATH") << "\"" << std::endl;
            std::cout << "SHELL: \"" << getenv("SHELL") << "\"" << std::endl;
            std::cout << "Detected CPU Cores: \"" << processorCount << "\"" << std::endl;
        }
        
        // By default, use the local user path to store package repositories
        const boost::filesystem::path prefix = context.package.path;
        const boost::filesystem::path userPath = getUserPath();
        const std::string suffix = computePathSuffix(params.triplet);

        PackageManager manager {
            *executor.get(),
            (prefix / ".Xenobuild").string(),
            suffix
        };
        
        DependencyManager dependencyManager {
            *executor.get(),
            (userPath / ".Xenobuild").string(),
            suffix
        };

        performImpl(manager, dependencyManager);
    }


    void ConfigureController::performImpl(PackageManager &packageManager, DependencyManager &dependencyManager) {
        const CMakeBuildType buildTypes[] = {
            CMakeBuildType::Release, CMakeBuildType::Debug
        };
        
        for (const CMakeBuildType buildType : buildTypes) {
            if (! packageManager.configure(context.package, context.toolchain, params.triplet, buildType, dependencyManager)) {
                throw std::runtime_error("Configure command failed.");
            }
        }
    }
}
