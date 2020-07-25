
#include "ConfigureController.hpp"

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <bok/core/Constants.hpp>
#include <bok/core/FileServiceImpl.hpp>
#include <bok/core/toolchain/Toolchain.hpp>
#include <bok/core/toolchain/ToolchainFactory_FS.hpp>
#include <bok/core/package/Package.hpp>
#include <bok/core/package/PackageRegistry.hpp>
#include <bok/core/package/Module.hpp>
#include <bok/core/package/PackageRegistryFactory.hpp>
#include <bok/core/package/FSPackageFactory.hpp>
#include <bok/core/pipeline/BuildCache.hpp>
#include <bok/feature/build/ConfigurationService.hpp>

namespace bok {
    struct ConfigureControllerOptions {
        bool showHelp = false;
        std::string helpMessage;

        std::string buildType;
        boost::optional<std::string> toolchain;
        boost::optional<std::string> toolchainPath;

        //! additional search path for package dependencies
        std::vector<std::string> searchPaths;

        boost::optional<boost::filesystem::path> sourcePath;
        boost::optional<boost::filesystem::path> outputPath;

        std::map<std::string, std::string> variables;
    };

    struct ConfigureController::Private {
        ConfigureControllerOptions parse(int argc, char **argv) {
            namespace po = boost::program_options;

            po::options_description desc("Allowed options for Configure subcommand");

            desc.add_options()
                ("help", "produce this message")
                ("build-type", po::value<std::string>(), "set build type (debug, release, all)")
                ("toolchain", po::value<std::string>(), "set toolchain (gcc, vc)")
                ("toolchain-path", po::value<std::string>(), "set toolchain installation path")
                ("search-path", po::value<std::string>(), "set current search path directory for packages(?)")
                ("var", po::value<std::vector<std::string>>(), "define required variable used by some packages")
            ;

            po::variables_map vm;
            po::store(parse_command_line(argc, argv, desc), vm);
            po::notify(vm);

            ConfigureControllerOptions options;

            if (vm.count("help")) {
                std::stringstream ss;

                ss << desc << "\n";

                options.showHelp = true;
                options.helpMessage = ss.str();
            }
            
            if (vm.count("build-type")) {
                options.buildType = vm["build-type"].as<std::string>();
            } else {
                options.buildType = "all";
            }

            if (vm.count("toolchain")) {
                options.toolchain = vm["toolchain"].as<std::string>();
            }

            if (vm.count("toolchain-path")) {
                options.toolchainPath = vm["toolchain-path"].as<std::string>();
            }

            if (vm.count("search-path")) {
                options.searchPaths.push_back(vm["search-path"].as<std::string>());
            }

            if (vm.count("var")) {
                auto vars = vm["var"].as<std::vector<std::string>>();

                for (const auto &var : vars) {
                    std::vector<std::string> keyValue;
                    boost::split(keyValue, var, boost::is_any_of(":"));

                    const size_t size = keyValue.size();

                    switch (size) {

                    case 2:
                        options.variables.insert({keyValue[0], keyValue[1]});
                        break;

                    case 3:
                        options.variables.insert({keyValue[0], keyValue[1] + ":" + keyValue[2]});
                        break;
                    }
                }
            }

            return options;
        }

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


        std::unique_ptr<PackageRegistry> createPackageRegistry(PackageFactory *packageService, const boost::filesystem::path &packageRegistryPath) const {
            PackageRegistryFactory factory;

            return factory.createPackageRegistry(packageService, packageRegistryPath);
        }
    };


    ConfigureController::ConfigureController() 
        : impl(new ConfigureController::Private()) {}

    ConfigureController::~ConfigureController() {
        delete impl;
    }


    void ConfigureController::perform(int argc, char **argv) {
        const auto options = impl->parse(argc, argv);
        
        if (options.showHelp) {
            std::cout << options.helpMessage;

            return;
        }

        const boost::filesystem::path basePackagePath = options.sourcePath 
            ? options.sourcePath.get()
            : boost::filesystem::current_path();

        const boost::filesystem::path outputPath = options.outputPath
            ? options.outputPath.get()
            : basePackagePath / ".bok";

        auto configurationService = ConfigurationService{outputPath, basePackagePath};
        auto configurationData = configurationService.getData();

        if (configurationData.buildConfigurations.size() == 0 && !options.toolchain) {
            throw std::runtime_error(
                "There is no configurations associated. Must select a build type and a toolchain.\n"
                "See 'bok configure --help' for details."
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

        auto factory = std::make_unique<ToolchainFactory_FS>("./toolchain/", installationPath);
        auto toolchain = factory->createToolchain(options.toolchain.get());

        // setup the configuration requested by the user
        auto config = BuildConfiguration{};
        config.toolchainId = options.toolchain.get();
        config.arch = impl->detectTargetArchitecture();
        config.buildTypes = impl->generateBuildTypes(toolchain, options.buildType);
        config.version = toolchain->detectVersion();

        if (installationPath) {
            config.toolchainPath = installationPath.get().string();
        }
        
        std::cout << "Detected compiler version: " << std::string(config.version) << std::endl;

        // construct the package with the current toolchain, in order grab dependency information
        const FileServiceImpl fileService;
        auto packageService = std::make_unique<FSPackageFactory>(&fileService);
        auto packageRegistry = impl->createPackageRegistry(packageService.get(), BOK_PACKAGE_SEARCH_PATH);
        auto package = packageService->createPackage(basePackagePath, packageRegistry.get());

        // validate required variables for dependencies againts supplied ones
        for (const Module *component : package->getModules()) {
            for (const Module *dependency : component->getDependencies()) {
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
