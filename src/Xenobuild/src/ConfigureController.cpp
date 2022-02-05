
#include <Xenobuild/ConfigureController.h>
#include <Xenobuild/Common.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Version.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageManager.h>


namespace Xenobuild {
    const boost::filesystem::path currentPath = CMAKE_CURRENT_SOURCE_DIR;
    
    ConfigureControllerInput ConfigureControllerInput::parse(int, char**) {
        ConfigureControllerInput result;
        
        result.sourceDir = currentPath.string();
        result.buildDir = (currentPath / ".Xenobuild").string();
        result.triplet = Triplet::nativeHost();
        
        return result;
    }
}


namespace Xenobuild {
    const char* ConfigureController::Name = "configure";
    
    ConfigureController::ConfigureController(Package &package, const ConfigureControllerInput& params)
        : package(package), params(params) {}

    void ConfigureController::perform() {
        std::cout << "ConfigureController::perform" << std::endl;

        const std::vector<Dependency> dependencies = package.dependencies;
        
        const unsigned processorCount = getProcessorCount();
        
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
        
        if (getHostOS() == OS::Windows) {
            const std::vector<std::string> toolchainPrefixPaths = enumerateVCInstallations();

            if (toolchainPrefixPaths.size() > 0) {
                toolchainPrefix = toolchainPrefixPaths[0];
            }
        }
        
        // By default, use the local user path to store package repositories
        const boost::filesystem::path prefix = getUserPath();
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

        std::for_each(dependencies.begin(), dependencies.end(), [&manager, &dependencies, this](const Dependency& dep) {
            std::cout << "Dependency " << dep.url << std::endl;
            // TODO: The default generator depends on the toolchain, and maybe, the platform.
            // const std::string generator = "NMake Makefiles";

            const CMakeBuildType buildTypes[] = {
                CMakeBuildType::Release, CMakeBuildType::Debug
            };
            
            for (const CMakeBuildType buildType : buildTypes) {
                if (! manager.configure(dependencies, package, this->params.triplet, buildType)) {
                    throw std::runtime_error("Configure command failed.");
                }
            }
        });
    }
}
