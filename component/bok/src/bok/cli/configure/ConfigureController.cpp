
#include "ConfigureController.hpp"
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/range/algorithm/find.hpp>

#include <borc/common/Constants.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/toolchain/ToolchainFactoryFS.hpp>
#include <borc/build/BuildCache.hpp>
#include <borc/build/ConfigurationService.hpp>
#include <borc/model/Version.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/PackageRegistry.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/PackageRegistryFactory.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/services/PackageServiceImpl.hpp>


namespace bok {
    struct ConfigureController::Private {
        /**
         * @brief Determine all the build types from the parameter, specially when "All is used".
         * @todo: The values generated should come from the currently selected toolchain
         */
        std::set<BuildType> generateBuildTypes(const Toolchain *, const std::string &buildTypeValue) const {
            if (buildTypeValue == "all") {
                return { BuildType{"Debug"}, BuildType{"Release"} };
            } else {
                return { BuildType{buildTypeValue} };
            }
        }


        /**
         * @brief Detect the current (native) architecture. 
         * @todo This information can be computed from a preprocessor directive.
         */
        std::string detectTargetArchitecture() const {
            return "x86_64";
        }


        std::unique_ptr<PackageRegistry> createPackageRegistry(PackageService *packageService, const boost::filesystem::path &packageRegistryPath) const {
            PackageRegistryFactory factory;

            return factory.createPackageRegistry(packageService, packageRegistryPath);
        }
    };


    ConfigureController::ConfigureController() 
        : m_impl(new ConfigureController::Private()) {}

    ConfigureController::~ConfigureController() {
        delete m_impl;
    }


    void ConfigureController::perform(const ConfigureControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;

            return;
        }

        const boost::filesystem::path basePackagePath = options.sourcePath 
            ? options.sourcePath.get()
            : boost::filesystem::current_path();

        const boost::filesystem::path outputPath = options.outputPath
            ? options.outputPath.get()
            : basePackagePath / ".borc";

        auto configurationService = ConfigurationService{outputPath, basePackagePath};
        auto configurationData = configurationService.getData();

        if (configurationData.buildConfigurations.size() == 0 && !options.toolchain) {
            throw std::runtime_error(
                "There is no configurations associated. Must select a build type and a toolchain.\n"
                "See 'borc configure --help' for details."
            );
        }

        if (!options.toolchain) {
            std::cout << "Configured builds for current package:" << std::endl;

            for (const auto &config : configurationData.buildConfigurations) {
                std::cout << "    " << config.computeIdentifier() << std::endl;
            }

            return;
        }

        std::cout << "Configuring build: type=" << options.buildType << ", toolchain=" << options.toolchain.get() << std::endl;

        boost::optional<boost::filesystem::path> installationPath;

        if (options.toolchainPath) {
            installationPath = boost::filesystem::path{ options.toolchainPath.get() };
        }

        auto factory = std::make_unique<ToolchainFactoryFS>("./toolchain/", installationPath);
        auto toolchain = factory->createToolchain(options.toolchain.get());

        // setup the configuration requested by the user
        auto config = BuildConfiguration{};
        config.toolchainId = options.toolchain.get();
        config.arch = m_impl->detectTargetArchitecture();
        config.buildTypes = m_impl->generateBuildTypes(toolchain, options.buildType);
        config.version = toolchain->detectVersion();

        if (installationPath) {
            config.toolchainPath = installationPath.get().string();
        }
        
        std::cout << "Detected compiler version: " << std::string(config.version) << std::endl;

        // construct the package with the current toolchain, in order grab dependency information
        const FileServiceImpl fileService;
        auto packageService = std::make_unique<PackageServiceImpl>(&fileService);
        auto packageRegistry = m_impl->createPackageRegistry(packageService.get(), BOK_PACKAGE_SEARCH_PATH);
        auto package = packageService->createPackage(basePackagePath, packageRegistry.get());

        // validate required variables for dependencies againts supplied ones
        for (const Module *module : package->getModules()) {
            for (const Module *dependency : module->getDependencies()) {
                std::vector<PackageVariable> variables = dependency->getPackage()->getVariables();

                for (const PackageVariable &variable : variables) {
                    if (auto varIt = options.variables.find(variable.name); varIt != options.variables.end()) {
                        config.variables.insert({varIt->first, varIt->second});
                    } else {
                        throw std::runtime_error("Required variable " + variable.name + " for dependent package " + dependency->getPackage()->getName() + " isn't defined. Use the --var=Name:Value configure option to set it.");
                    }
                }
            }
        }

        configurationService.addBuildConfiguration(config);

        configurationService.saveAllBuildConfigurations();

        std::cout << "Configuration done. " << std::endl;
    }
}
