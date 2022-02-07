
#include <Xenobuild/core/Util.h>

#include <thread>
#include <boost/filesystem.hpp>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Triplet.h>
#include <Xenobuild/core/Dependency.h>

namespace Xenobuild {
    boost::optional<std::string> getenv(const std::string& var) {
        const char* val = std::getenv(var.c_str());

        if (val == nullptr) {
            return {};
        }

        return std::string(val);
    }

    unsigned getProcessorCount() {
        const auto processor_count = std::thread::hardware_concurrency();
        
        if (processor_count == 0) {
            return 1;
        }
        
        return processor_count;
    }

    std::string quote(const std::string& str) {
        return "\"" + str + "\"";
    }
    
    std::ostream& write(std::ostream &os, std::vector<std::string> lines) {
        for (const std::string &line : lines) {
            os << line << std::endl;
        }
        
        return  os;
    }
    
    std::string evaluate(const CMakeDefinition& def) {
        return "-D" + def.name + "=" + quote(def.value);
    }
    
    std::vector<std::string> enumerateVCInstallations() {
        const std::vector<std::string> vars = {
            "VS2019INSTALLDIR"
        };

        std::vector<std::string> installations;

        for (const std::string& var : vars) {
            const boost::optional<std::string> value = getenv(var);

            if (! value.has_value()) {
                continue;
            }

            installations.push_back(value.get());
        }

        return installations;
    }

    CommandX createVCVars64Command(const boost::filesystem::path &prefixPath) {
        const auto vcvars = prefixPath / "VC\\Auxiliary\\Build\\vcvars64.bat";

        return { "call", { quote(vcvars.string()) } };
    }

    CommandX generateCommand(const CMakeConfig &config) {
        CommandX command {
            "cmake",
            {
                "-S" + quote(config.sourcePath),
                "-B" + quote(config.buildPath)
            }
        };

        if (config.generator != "") {
            const std::string arg = "-G " + quote(config.generator);
            command.args.push_back(arg);
        }
        
        std::transform(
            config.definitions.begin(),
            config.definitions.end(),
            std::back_inserter(command.args), [](const auto pair) {

            const CMakeDefinition definition{ pair.first, pair.second };

            return evaluate(definition);
        });

        return command;
    }

    
    CommandX generateCommand(const CMakeBuild& build) {
        CommandX command {
            "cmake",
            {"--build", build.buildPath}
        };
        
        if (build.jobCount) {
            // this require CMake >= 3.12.
            command.args.push_back("--parallel " + std::to_string(*build.jobCount));
        }
        
        return command;
    }

    
    CommandX generateCommand(const CMakeInstall& install) {
        return { "cmake", {"--install", install.buildPath } };
    }

    
    std::string evaluate(const CMakeBuildType buildType) {
        switch (buildType) {
        case CMakeBuildType::Default:
            return "";

        case CMakeBuildType::Debug:
            return "Debug";

        case CMakeBuildType::Release:
            return "Release";

        default:
            return "";
        }
    }


    std::map<std::string, std::string> createConfigDefinitions(const boost::filesystem::path& installPrefix, const CMakeBuildType buildType) {
        std::map<std::string, std::string> definitions = {
            // requiered in macOS; unused on other platforms
            {"CMAKE_OSX_ARCHITECTURES", "arm64;x86_64"},
            
            // for generating dual-config libraries (Debug and Release)
            {"CMAKE_DEBUG_POSTFIX", "d"},
        };

        if (installPrefix != "") {
            definitions["CMAKE_INSTALL_PREFIX"] = installPrefix.string();
        }
        
        if (buildType != CMakeBuildType::Default) {
            definitions["CMAKE_BUILD_TYPE"] = evaluate(buildType);
        }

        return definitions;
    }
    
    CommandBatch createCMakeBatch(const CommandX command, const boost::filesystem::path &toolchainPrefix) {
        CommandBatch batch{};

        if (getHostOS() == OS::Windows) {
            const CommandX vcvars = createVCVars64Command(toolchainPrefix);
            batch.commands.push_back(vcvars);
        }

        batch.commands.push_back(command);

        return batch;
    }

    boost::filesystem::path getUserPath() {
        boost::optional<std::string> userPathStr;

        switch (getHostOS()) {
            case OS::Windows:
                userPathStr = getenv("USERPROFILE");
                break;
                
            case OS::MacOS:
            case OS::Linux:
                userPathStr = getenv("HOME");
                break;
        }

        if (!userPathStr) {
            return boost::filesystem::current_path();
        }

        return userPathStr.get();
    }
}
