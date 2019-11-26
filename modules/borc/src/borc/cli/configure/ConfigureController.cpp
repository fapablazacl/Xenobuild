
#include "ConfigureController.hpp"
#include <iostream>

#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <borc/model/Version.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/toolchain/ToolchainFactoryImpl.hpp>
#include <borc/build/BuildCache.hpp>
#include <borc/build/ConfigurationService.hpp>
#include <borc/model/Package.hpp>
#include <borc/model/PackageRegistry.hpp>
#include <borc/model/Module.hpp>
#include <borc/services/FileServiceImpl.hpp>
#include <borc/services/PackageServiceImpl.hpp>

namespace borc {
    ConfigureController::~ConfigureController() {}


    void ConfigureController::perform(const ConfigureControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;

            return;
        }

        PackageRegistry packageRegistry;

        const boost::filesystem::path basePackagePath = options.sourcePath 
            ? options.sourcePath.get()
            : boost::filesystem::current_path();

        const boost::filesystem::path outputPath = options.outputPath
            ? options.outputPath.get()
            : basePackagePath / ".borc";

        auto configurationService = ConfigurationService{outputPath};
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

        auto factory = std::make_unique<ToolchainFactoryImpl>("toolchain");
        auto toolchain = factory->createToolchain(options.toolchain.get());

        // setup the configuration requested by the user
        auto config = BuildConfiguration{};
        config.toolchainId = options.toolchain.get();
        config.arch = this->detectArchitecture();
        config.buildTypes = this->generateBuildTypes(toolchain, options.buildType);
        config.version = this->detectToolchainVersion();

        std::cout << "Detected compiler version: " << std::string(config.version) << std::endl;

        configurationService.addBuildConfiguration(config);

        // construct the package with the current toolchain, in order grab dependency information
        const FileServiceImpl fileService;
        auto packageService = std::make_unique<PackageServiceImpl>(&fileService);
        auto package = packageService->createPackage(basePackagePath, &packageRegistry);

        configurationService.saveAllBuildConfigurations();
    }


    Version ConfigureController::detectToolchainVersion() const {
        // 1. Compile C++ version detector
        if (std::system("gcc other/CXXCompilerVersionDetector.cpp -O0 -oother/CXXCompilerVersionDetector") != 0) {
            throw std::runtime_error("Failed CXXCompilerVersionDetector compilation.");
        }

        // 2. Execute it, and grab the output
        boost::filesystem::path compilerPath = boost::filesystem::path("./other/CXXCompilerVersionDetector");

        if (! boost::filesystem::exists(compilerPath)) {
            throw std::runtime_error("Compiler detector not found in path '" + compilerPath.string() + "'.");
        }

        boost::process::ipstream pipeStream;
        boost::process::child childProcess {compilerPath, boost::process::std_out > pipeStream};
        
        std::string line;
        std::vector<std::string> specs;

        while (pipeStream && std::getline(pipeStream, line) && !line.empty()) {
            specs.push_back(line);
        }

        childProcess.wait();

        if (specs.size() != 1) {
            throw std::runtime_error("Couldn't detect compiler type and version (unexpected output)");
        }

        std::vector<std::string> compilerDetectorOutput;

        boost::algorithm::split(compilerDetectorOutput, specs[0], boost::is_any_of("-"));

        if (compilerDetectorOutput.size() != 2) {
            throw std::runtime_error("Couldn't detect compiler type and version (unexpected output)");
        }

        // 3. Parse the output and return the result.
        return Version::parse(compilerDetectorOutput[1]);
    }


    /**
     * @brief Determine all the build types from the parameter, specially when "All is used".
     * @todo: The values generated should come from the currently selected toolchain
     */
    std::set<BuildType> ConfigureController::generateBuildTypes(const Toolchain *, const std::string &buildTypeValue) const {
        if (buildTypeValue == "all") {
            return { BuildType{"Debug"},BuildType{"Release"} };
        } else {
            return { BuildType{buildTypeValue} };
        }
    }


    /**
     * @brief Detect the current (native) architecture. 
     * @todo This information can be computed from a preprocessor directive.
     */
    std::string ConfigureController::detectArchitecture() const {
        return "x86_64";
    }
}
