
#include <Xenobuild/core/CMakeBuildSystem.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Util.h>
#include <Xenobuild/core/Triplet.h>


namespace Xenobuild {
    std::string evaluate(const CMakeDefinition& def) {
        return "-D" + def.name + "=" + quote(def.value);
    }
    
    std::string evaluate(const CMakeGenerator generator) {
        switch (generator) {
        case CMakeGenerator::MinGWMakefiles:
            return "MinGW Makefiles";

        case CMakeGenerator::NMakeMakefiles:
            return "NMake Makefiles";

        case CMakeGenerator::UnixMakefiles:
            return "Unix Makefiles";
        }

        return "";
    }

    CommandX generateCommand(const CMakeConfig &config) {
        CommandX command {
            "cmake",
            {
                "-S" + quote(config.sourcePath),
                "-B" + quote(config.buildPath)
            }
        };

        if (config.generator) {
            const std::string arg = "-G " + quote(evaluate(config.generator.get()));
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
        
        //if (build.jobCount) {
        //    // this require CMake >= 3.12.
        //    command.args.push_back("--parallel " + std::to_string(*build.jobCount));
        //}
        
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
            // requiered in macOS; unused in other platforms
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
}
