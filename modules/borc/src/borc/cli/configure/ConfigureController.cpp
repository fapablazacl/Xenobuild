
#include "ConfigureController.hpp"
#include <iostream>

#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <borc/model/Version.hpp>
#include <borc/toolchain/Toolchain.hpp>
#include <borc/toolchain/ToolchainFactory.hpp>
#include <borc/build/BuildCache.hpp>
#include <borc/build/ConfigurationService.hpp>

namespace borc {
    ConfigureController::~ConfigureController() {}

    /**
     * @brief Determine all the build types from the parameter, specially when "All is used".
     * @todo: The values generated should come from the currently selected toolchain
     */
    static std::set<BuildType> generateBuildTypes(const Toolchain *, const std::string &buildTypeValue) {
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
    static std::string detectArchitecture() {
        return "x86_64";
    }

    void ConfigureController::perform(const ConfigureControllerOptions &options) {
        if (options.showHelp) {
            std::cout << options.helpMessage;

            return;
        }

        const auto packagePath = boost::filesystem::current_path();
        if (! boost::filesystem::exists(packagePath / "borc-package.json")) {
            throw std::runtime_error("There is no package file in the folder '" + packagePath.string() + "'");
        }

        const auto outputPath = packagePath / ".borc";

        auto configurationService = ConfigurationService{outputPath};
        auto configurationData = configurationService.getData();

        if (configurationData.buildConfigurations.size() == 0 && !options.buildType && !options.toolchain) {
            throw std::runtime_error(
                "There is no configurations associated. Must select a build type and a toolchain.\n"
                "See 'borc configure --help' for details."
            );
        }

        if (!options.buildType && !options.toolchain) {
            std::cout << "Configured builds for current package:" << std::endl;

            for (const auto &config : configurationData.buildConfigurations) {
                std::cout << "    " << config.computeIdentifier() << std::endl;
            }

            return;
        }

        std::cout << "Configuring build: type=" << options.buildType.get() << ", toolchain=" << options.toolchain.get() << std::endl;

        auto factory = ToolchainFactory::create();
        auto toolchain = factory->createToolchain(options.toolchain.get());

        // setup the configuration requested by the user
        auto config = BuildConfiguration{};
        config.toolchainId = options.toolchain.get();
        config.arch = detectArchitecture();
        config.buildTypes = generateBuildTypes(toolchain.get(), options.buildType.get());
        config.version = detectToolchainVersion();

        std::cout << "Detected compiler version: " << std::string(config.version) << std::endl;

        configurationService.addBuildConfiguration(config);
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
}
