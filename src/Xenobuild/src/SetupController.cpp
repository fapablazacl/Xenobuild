
// SetupController


#include <Xenobuild/SetupController.h>

#include <boost/program_options.hpp>
#include <boost/optional/optional_io.hpp>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Version.h>
#include <Xenobuild/core/Context.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageFactory.h>
#include <Xenobuild/core/Module.h>
#include <Xenobuild/core/Dependency.h>
#include <Xenobuild/core/DependencyManager.h>
#include <Xenobuild/core/Toolchain.h>
#include <Xenobuild/core/CMakeBuildSystem.h>



namespace Xenobuild {
    SetupControllerInput SetupControllerInput::parse(const std::vector<std::string> &args) {
        boost::program_options::options_description desc("setup options");
        desc.add_options()
            // ("hidden", "Show hidden files")
            ("toolchain", boost::program_options::value<std::string>(), "The toolchain identifier to use while building dependencies");

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(args).options(desc).run(), vm);

        if (vm["toolchain"].empty()) {
            throw std::runtime_error("--toolchain option is mandatory.");
        }

        auto toolchainTypeOpt = decode(vm["toolchain"].as<std::string>());

        if (!toolchainTypeOpt) {
            throw std::runtime_error("Unknown toolchain.");
        }

        const auto currentPath = boost::filesystem::current_path();

        SetupControllerInput result;
        result.triplet.toolchainType = toolchainTypeOpt.get();
        result.sourceDir = currentPath.string();
        result.buildDir = (currentPath / ".Xenobuild").string();
        
        return result;
    }
}

namespace Xenobuild {
    const char* SetupController::Name = "setup";
    
    SetupController::SetupController(Context& context, const SetupControllerInput& params)
        : context(context), params(params) {}


    void SetupController::perform() {
        const std::vector<Dependency> dependencies = context.package.dependencies;
        
        const unsigned processorCount = getProcessorCount();
        
        // show current execution environment
        if (params.showEnvironment) {
            std::cout << "USER: \"" << getenv("USER") << "\"" << std::endl;
            std::cout << "PATH: \"" << getenv("PATH") << "\"" << std::endl;
            std::cout << "SHELL: \"" << getenv("SHELL") << "\"" << std::endl;
            std::cout << "Detected CPU Cores: \"" << processorCount << "\"" << std::endl;
        }
        
        // By default, use the local user path to store package repositories
        const boost::filesystem::path userPath = getUserPath();
        const std::string suffix = computePathSuffix(params.triplet);

        // BoostProcessCommandExecutor executor;
        SystemCommandExecutor executor;
        DependencyManager manager {
            executor,
            (userPath / ".Xenobuild").string(),
            suffix,
            processorCount
        };

        std::for_each(dependencies.begin(), dependencies.end(), [&manager, this](const Dependency& dep) {
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
                if (!manager.configure(dep, context.toolchain, buildType, {})) {
                    throw std::runtime_error("Configure command failed.");
                }
                
                std::cout << "Done." << std::endl;
                
                std::cout << "    Building " << evaluate(buildType) << "... ";
                if (! manager.build(dep, context.toolchain, buildType)) {
                    throw std::runtime_error("Build command failed.");
                }
                std::cout << "Done." << std::endl;
                
                std::cout << "    Installing " << evaluate(buildType) << "...";
                if (! manager.install(dep, context.toolchain, buildType)) {
                    throw std::runtime_error("Install command failed.");
                }
                std::cout << "Done." << std::endl;
            }
        });
    }
}
