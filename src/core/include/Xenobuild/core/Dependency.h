#pragma once 



namespace Xenobuild {
    struct GitRepository {
        std::string url;
        std::string tag;

        GitRepository(const std::string& url, const std::string& tag)
            : url(url), tag(tag) {}

        CommandResult clone(CommandExecutor &execute, const boost::filesystem::path &sourcePath) const {
            if (boost::filesystem::exists(sourcePath)) {
                return {0, {}, {}};
            }

            CommandX command{
                "git", {"clone"}
            };

            if (tag != "") {
                command.args.push_back("--branch " + tag);
            }
            
            command.args.push_back("--depth 1");
            command.args.push_back(url);
            command.args.push_back(sourcePath.string());

            return execute(command);
        }
    };

    struct CMakeDefinition {
        std::string name;
        std::string value;
    };

    inline std::string evaluate(const CMakeDefinition& def) {
        return "-D" + def.name + "=" + quote(def.value);
    }

    struct CMakeConfig {
        std::string sourcePath;
        std::string buildPath;
        std::string generator;
        std::map<std::string, std::string> definitions;
    };


    inline std::vector<std::string> enumerateVCInstallations() {
        const std::vector<std::string> vars = {
            "VS2019INSTALLDIR"
        };

        std::vector<std::string> installations;

        for (const std::string& var : vars) {
            const char* value = std::getenv(var.c_str());

            if (value == nullptr) {
                continue;
            }

            installations.push_back(value);
        }

        return installations;
    }

    inline CommandX createVCVars64Command(const boost::filesystem::path &prefixPath) {
        const auto vcvars = prefixPath / "VC\\Auxiliary\\Build\\vcvars64.bat";

        return { "call", { quote(vcvars.string()) } };
    }

    inline CommandX generateCommand(const CMakeConfig &config) {
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

    
    struct CMakeBuild {
        std::string buildPath;
        boost::optional<unsigned> jobCount;
    };

    inline CommandX generateCommand(const CMakeBuild& build) {
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

    
    struct CMakeInstall {
        std::string buildPath;
    };
    
    inline CommandX generateCommand(const CMakeInstall& install) {
        return { "cmake", {"--install", install.buildPath } };
    }

    enum class CMakeBuildType {
        Default,
        Debug,
        Release
    };


    inline std::string evaluate(const CMakeBuildType buildType) {
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


    struct Dependency {
        //! the Git URL repository
        std::string url;

        //! the tag (or branch), of the Git repository.
        std::string tag;

        //! version string. used to assemble the installation path suffix.
        std::string version;

        //! cmake definitions, in the dependency build/install stage.
        std::map<std::string, std::string> definitions;

        Dependency() {}

        Dependency(const std::string& url)
            : url(url) {}

        Dependency(const std::string& url, const std::string &tag, const std::string &version)
            : url(url), tag(tag), version(version) {}

        Dependency(const std::string& url, const std::string &tag, const std::string &version, const std::map<std::string, std::string> &definitions)
            : url(url), tag(tag), version(version), definitions(definitions) {}
        
        Dependency(const std::string& url, const std::map<std::string, std::string> &definitions)
            : url(url), definitions(definitions) {}
    };

    inline std::map<std::string, std::string> createConfigDefinitions(const boost::filesystem::path& installPrefix, const CMakeBuildType buildType) {
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
    
    inline CommandBatch createCMakeBatch(const CommandX command, const boost::filesystem::path &toolchainPrefix) {
        CommandBatch batch{};

        if (getHostOS() == OS::Windows) {
            const CommandX vcvars = createVCVars64Command(toolchainPrefix);
            batch.commands.push_back(vcvars);
        }

        batch.commands.push_back(command);

        return batch;
    }    
}
