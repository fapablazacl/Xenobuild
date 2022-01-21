
// SetupController


#include <Xenobuild/SetupController.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Version.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageFactory.h>
#include <Xenobuild/core/Module.h>
#include <Xenobuild/core/SourceFile.h>

namespace Xenobuild {
    SetupControllerInput SetupControllerInput::parse(int, char**) {
        const auto currentPath = boost::filesystem::current_path();

        SetupControllerInput result;

        result.sourceDir = currentPath.string();
        result.buildDir = (currentPath / ".Xenobuild").string();
        result.triplet = Triplet::nativeHost();
        
        return result;
    }
}

namespace Xenobuild {
    
    const char* SetupController::Name = "setup";
    
    SetupController::SetupController(PackageFactory &packageFactory, const SetupControllerInput& params) 
        : packageFactory(packageFactory), params(params) {}


    void SetupController::perform() {
        const std::vector<Dependency> dependencies = {
            Dependency{
                "https://github.com/glfw/glfw.git", 
                "3.3", "3.3",
                {
                    { "GLFW_BUILD_DOCS", "OFF" },
                    { "GLFW_BUILD_EXAMPLES", "OFF" },
                    { "GLFW_BUILD_TESTS", "OFF" }
                }
            },
            Dependency {
                "https://github.com/jbeder/yaml-cpp.git", 
                "yaml-cpp-0.7.0", "0.7.0",
                {
                    { "YAML_CPP_BUILD_TESTS", "OFF" }
                }
            },
            Dependency{
                "https://github.com/catchorg/Catch2.git",
                "v3.0.0-preview3", "3.0.0-rc3",
                {
                    { "CATCH_BUILD_TESTING", "OFF" },
                    { "CATCH_INSTALL_DOCS", "OFF" }
                }
            },
            Dependency{ "https://github.com/fapablazacl/glades2.git" },
            Dependency{
                "https://github.com/cginternals/glbinding.git",
                "v3.1.0", "3.1.0",
                {
                    { "OPTION_BUILD_EXAMPLES", "OFF" },
                    { "OPTION_BUILD_TOOLS", "OFF" },
                    { "BUILD_SHARED_LIBS", "ON" }
                }
            },

            // build fails in Windows.
            //Dependency {
            //    "https://github.com/google/fruit.git", 
            //    {
            //        { "FRUIT_TESTS_USE_PRECOMPILED_HEADERS", "OFF" },
            //        { "FRUIT_USES_BOOST", "OFF" }
            //    }
            //},
        };
        
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
        DependencyManager manager {
            executor,
            (prefix / ".Xenobuild").string(),
            toolchainPrefix,
            suffix,
            processorCount
        };

        std::for_each(dependencies.begin(), dependencies.end(), [&manager](const Dependency& dep) {
            std::cout << "Dependency " << dep.url << std::endl;
            // TODO: The default generator depends on the toolchain, and maybe, the platform.
            // const std::string generator = "NMake Makefiles";

            const CMakeBuildType buildTypes[] = {
                CMakeBuildType::Release, CMakeBuildType::Debug
            };

            std::cout << "    Downloading... ";
            if (!manager.download(dep)) {
                throw std::runtime_error("Download command failed.");
            }
            std::cout << "Done." << std::endl;

            for (const CMakeBuildType buildType : buildTypes) {
                std::cout << "    Configuring " << evaluate(buildType) << "... ";
                // manager.configure(dep, buildType, generator);
                if (! manager.configure(dep, buildType, "")) {
                    throw std::runtime_error("Configure command failed.");
                }
                
                std::cout << "Done." << std::endl;
                
                std::cout << "    Building " << evaluate(buildType) << "... ";
                if (! manager.build(dep, buildType)) {
                    throw std::runtime_error("Build command failed.");
                }
                std::cout << "Done." << std::endl;
                
                std::cout << "    Installing " << evaluate(buildType) << "...";
                if (! manager.install(dep, buildType)) {
                    throw std::runtime_error("Install command failed.");
                }
                std::cout << "Done." << std::endl;
            }
        });
    }
}
