
// SetupController


#include <Xenobuild/SetupController.h>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Version.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageFactory.h>
#include <Xenobuild/core/Module.h>
#include <Xenobuild/core/SourceFile.h>

#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>
#include <boost/optional.hpp>


namespace Xenobuild {
    constexpr Platform getHostPlatform() {
#if defined (_WIN32)
        return Platform::Windows;
#else
#error Current Platform isn't supported.
#endif
    }


    class CommandExecutorException : public std::runtime_error {
    public:
        CommandExecutorException(const std::string& msg, const int exitCode) 
            : runtime_error(msg), exitCode(exitCode) {}

        int getExitCode() const {
            return exitCode;
        }

    private:
        const int exitCode;
    };
    
    struct CommandX {
        std::string name;
        std::vector<std::string> args;
    };

    struct CommandBatch {
        CommandBatch() {}

        CommandBatch(std::string name, std::vector<std::string> args) {
            commands.push_back(CommandX{name, args});
        }

        CommandBatch(const std::vector<CommandX> &commands) : commands(commands) {}

        CommandBatch(const CommandX &command) {
            commands.push_back(command);
        }

        std::vector<CommandX> commands;
    };

    class CommandExecutor {
    public:
        virtual ~CommandExecutor() {}

        virtual void execute(const CommandBatch& batch) = 0;

        virtual void execute(const CommandX& command) = 0;

        inline void operator() (const CommandX& command) {
            execute(command);
        }

        inline void operator() (const CommandBatch& batch) {
            execute(batch);
        }
    };

    class SystemCommandExecutor : public CommandExecutor {
    public:
        virtual ~SystemCommandExecutor() {}

        void execute(const CommandX& command) override {
            const std::string cmdline = createCmdLine(command);
            const int code = std::system(cmdline.c_str());

            if (code != 0) {
                const std::string msg = "Error while executing command '" + cmdline + "'";
                throw CommandExecutorException(msg, code);
            }
        }

        void execute(const CommandBatch& batch) override {
            std::vector<std::string> cmdlines;

            std::transform(
                batch.commands.begin(), 
                batch.commands.end(), 
                std::back_inserter(cmdlines), [this](const auto command) {
                return createCmdLine(command);
            });

            const std::string cmdline = boost::join(cmdlines, "&&");

            const int code = std::system(cmdline.c_str());

            if (code != 0) {
                const std::string msg = "Error while executing command batch:'" + cmdline + "'";
                throw CommandExecutorException(msg, code);
            }
        }

    private:
        std::string createCmdLine(const CommandX& command) const {
            const std::string cmdline = command.name + " " + boost::join(command.args, " ");

            return cmdline;
        }
    };

    class BoostProcessCommandExecutor : public CommandExecutor {
    public:
        virtual ~BoostProcessCommandExecutor() {}

        void execute(const CommandX& command) override {
            boost::process::child childp;
        }
    };

    struct Url {
        std::string schema;
        std::string host;
        std::string path;
        // TODO: query string

        std::string str;

        static boost::optional<Url> parse(const std::string& strUrl) {
            // Sample URL: https://github.com/glfw/glfw.git
            Url url;

            const size_t schemaPos = strUrl.find_first_of(':');
            
            if (schemaPos == std::string::npos) {
                return {};
            }

            url.schema = strUrl.substr(0, schemaPos);

            const size_t hostEndPos = strUrl.find('/', schemaPos + 3);
            if (hostEndPos == std::string::npos) {
                return {};
            }

            url.host = strUrl.substr(schemaPos + 3, hostEndPos - (schemaPos + 3));

            url.path = strUrl.substr(hostEndPos);

            url.str = strUrl;

            return url;
        }

        std::string string() const {
            return str;
        }
    };

    struct GitRepository {
        std::string url;
        std::string tag;

        GitRepository(const std::string& url, const std::string& tag) 
            : url(url), tag(tag) {}

        void clone(CommandExecutor &execute, const boost::filesystem::path &sourcePath) const {
            if (boost::filesystem::exists(sourcePath)) {
                return;
            }

            CommandX command{
                "git", {"clone", "--depth 1", url, sourcePath.string()}
            };

            if (tag != "") {
                command.args.push_back("--branch " + tag);
            }

            execute(command);
        }
    };

    struct CMakeDefinition {
        std::string name;
        std::string value;
    };

    inline std::string quote(const std::string& str) {
        return "\"" + str + "\"";
    }

    std::string evaluate(const CMakeDefinition& def) {
        return "-D" + def.name + "=" + quote(def.value);
    }

    struct CMakeConfig {
        std::string sourcePath;
        std::string buildPath;
        std::string generator;
        std::map<std::string, std::string> definitions;
    };


    std::vector<std::string> enumerateVCInstallations() {
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

    CommandX createVCVars64Command(const boost::filesystem::path &prefixPath) {
        const auto vcvars = prefixPath / "VC\\Auxiliary\\Build\\vcvars64.bat";

        return { "call", { quote(vcvars.string()) } };
    }

    CommandX generateCommand(const CMakeConfig &config) {
        CommandX command {
            "cmake",
            {
                "-S" + config.sourcePath,
                "-B" + config.buildPath,
                "-G " + quote(config.generator)
            }
        };

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
    };

    CommandX generateCommand(const CMakeBuild& build) {
        return { "cmake", {"--build", build.buildPath} };
    }

    
    struct CMakeInstall {
        std::string buildPath;
    };
    
    CommandX generateCommand(const CMakeInstall& install) {
        return { "cmake", {"--install", install.buildPath } };
    }

    enum class CMakeBuildType {
        Default,
        Debug,
        Release
    };


    std::string evaluate(const CMakeBuildType buildType) {
        switch (buildType) {
        case CMakeBuildType::Debug:
            return "Debug";

        case CMakeBuildType::Release:
            return "Release";
        }

        return "";
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


    class DependencyManager {
    public:
        explicit DependencyManager(CommandExecutor &executor, const std::string& prefixPath, const std::string &toolchainPrefix, const std::string &installSuffix) 
            : executor(executor), prefixPath(prefixPath), toolchainPrefix(toolchainPrefix), installSuffix(installSuffix) {}

        bool download(const Dependency& dependency) const {
            const auto repository = GitRepository { dependency.url, dependency.tag };
            const auto sourcePath = computePath(prefixPath / "sources", *Url::parse(dependency.url), dependency.tag);

            repository.clone(executor, sourcePath);

            return true;
        }

        bool configure(const Dependency& dependency, const CMakeBuildType buildType, const std::string &generator) {
            const auto sourcePath = computePath(prefixPath / "sources", *Url::parse(dependency.url), dependency.tag);
            
            const auto buildPath = computePath(sourcePath, buildType);
            const auto installPath = computePath(prefixPath / "packages" / installSuffix, *Url::parse(dependency.url), dependency.version);

            CMakeConfig config {
                sourcePath.string(),
                buildPath.string(),
                generator,
                createConfigDefinitions(installPath, buildType)
            };

            config.definitions.insert(dependency.definitions.begin(), dependency.definitions.end());

            CommandX command = generateCommand(config);
            CommandBatch batch = createCMakeBatch(command);

            executor(batch);

            return true;
        }

        bool build(const Dependency& dependency, const CMakeBuildType buildType) {
            const auto sourcePath = computePath(prefixPath / "sources", *Url::parse(dependency.url), dependency.tag);
            const auto buildPath = computePath(sourcePath, buildType);
            
            CMakeBuild build { buildPath.string() };
            CommandX command = generateCommand(build);
            CommandBatch batch = createCMakeBatch(command);

            executor(batch);

            return true;
        }

        bool install(const Dependency& dependency, const CMakeBuildType buildType) {
            const auto sourcePath = computePath(prefixPath / "sources", *Url::parse(dependency.url), dependency.tag);
            const auto buildPath = computePath(sourcePath, buildType);
            
            CMakeInstall install { buildPath.string() };
            CommandX command = generateCommand(install);
            CommandBatch batch = createCMakeBatch(command);

            executor(batch);

            return true;
        }


    private:
        CommandBatch createCMakeBatch(const CommandX command) {
            CommandBatch batch{};

            if (getHostPlatform() == Platform::Windows) {
                const CommandX vcvars = createVCVars64Command(toolchainPrefix);
                batch.commands.push_back(vcvars);
            }

            batch.commands.push_back(command);

            return batch;
        }

        std::map<std::string, std::string> createConfigDefinitions(const boost::filesystem::path& installPrefix, const CMakeBuildType buildType) {
            std::map<std::string, std::string> definitions = {
                {"CMAKE_OSX_ARCHITECTURES", "arm64;x86_64"},
                {"CMAKE_DEBUG_POSTFIX", "d"},
                {"CMAKE_INSTALL_PREFIX", installPrefix.string()}
            };

            if (buildType != CMakeBuildType::Default) {
                definitions["CMAKE_BUILD_TYPE"] = evaluate(buildType);
            }

            return definitions;
        }


        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const CMakeBuildType type) const {
            switch (type) {
            case CMakeBuildType::Default:
                return prefix;

            case CMakeBuildType::Debug:
                return prefix / "debug";

            case CMakeBuildType::Release:
                return prefix / "release";
            }

            return prefix;
        }


        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const Url url, const std::string &suffix) const {
            boost::filesystem::path sourcePath{ prefix / url.host };

            std::vector<std::string> pathParts;
            boost::split(pathParts, url.path, boost::is_any_of("/"));

            //for (const auto& part : pathParts) {
            //    sourcePath /= part;
            //}

            //sourcePath /= boost::join_if(pathParts, "-", [](const auto part) {
            //    return part != "";
            //});
            const boost::filesystem::path dependencyName { pathParts[pathParts.size() - 1] };

            sourcePath /= dependencyName.stem();    // removes the .git suffix

            if (suffix != "") {
                sourcePath += ("-" + suffix);
            }

            return sourcePath;
        }

    private:
        CommandExecutor &executor;
        boost::filesystem::path prefixPath;
        boost::filesystem::path toolchainPrefix;
        std::string installSuffix;
    };


    std::string computePathSuffix(const Platform platform) {
        switch (platform) {
        case Platform::Windows:
            return "windows";

        case Platform::Linux:
            return "linux";

        case Platform::MacOS:
            return "macOS";
        }

        return "host";
    }

    
    std::string computePathSuffix(const Arch arch) {
        switch (arch) {
        case Arch::X32:
            return "x86";

        case Arch::X64:
            return "x64";
        }

        return "native";
    }
    

    std::string computePathSuffix(const Toolchain toolchain) {
        switch (toolchain) {
        case Toolchain::AppleClang:
            return "appleclang";

        case Toolchain::Clang:
            return "clang";

        case Toolchain::MicrosoftVC:
            return "vc";
            
        case Toolchain::GnuGCC:
            return "gcc";
        }

        return "cc";
    }

    std::string computePathSuffix(const Triplet& triplet) {
        return 
            computePathSuffix(triplet.platform) + "-" + 
            computePathSuffix(triplet.arch) + "-" + 
            computePathSuffix(triplet.toolchain);
    }
}


namespace Xenobuild {
    SetupControllerInput SetupControllerInput::parse(int, char**) {
        const auto currentPath = boost::filesystem::current_path();

        SetupControllerInput result;

        result.sourceDir = currentPath.string();
        result.buildDir = (currentPath / ".Xenobuild").string();

        result.triplet = {
            Platform::Windows,
            Arch::X64,
            Toolchain::MicrosoftVC
        };

        return result;
    }

    /**
     * @brief Get the home folder of the current user.
     * @todo refactor into a cross-platform solution.
     */
    std::string getUserPath() {
        return std::getenv("USERPROFILE");
    }
}


namespace Xenobuild {
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
            // fails build.
            //Dependency {
            //    "https://github.com/google/fruit.git", 
            //    {
            //        { "FRUIT_TESTS_USE_PRECOMPILED_HEADERS", "OFF" },
            //        { "FRUIT_USES_BOOST", "OFF" }
            //    }
            //},
        };

        // Pick a Default Toolchain
        const std::vector<std::string> toolchainPrefixPaths = enumerateVCInstallations();

        if (toolchainPrefixPaths.size() == 0) {
            // No toolchain found
            // ERROR;
            return;
        }

        const std::string toolchainPrefix = toolchainPrefixPaths[0];

        // By default, use the local user path to store package repositories
        const std::string prefix = getUserPath();
        const std::string suffix = computePathSuffix(params.triplet);

        SystemCommandExecutor executor;
        DependencyManager manager{executor, prefix + "\\.Xenobuild", toolchainPrefix, suffix};

        std::for_each(dependencies.begin(), dependencies.end(), [&manager](const Dependency& dep) {
            // TODO: The default generator depends on the toolchain, and maybe, the platform.
            const std::string generator = "NMake Makefiles";

            const CMakeBuildType buildTypes[] = {
                CMakeBuildType::Release, CMakeBuildType::Debug
            };

            manager.download(dep);

            for (const CMakeBuildType buildType : buildTypes) {
                manager.configure(dep, buildType, generator);
                manager.build(dep, buildType);
                manager.install(dep, buildType);
            }
        });
    }
}
