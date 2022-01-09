
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
#include <boost/graph/adjacency_list.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>


namespace Xenobuild {
    enum class Platform {
        Unknown,
        Windows,
        Linux,
        MacOS
    };

    constexpr Platform getHostPlatform() {
#if defined (_WIN32)
        return Platform::Windows;
#else
        return Platform::Unknown;
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


    struct URL {
        std::string schema;
        std::string host;
        std::string path;
        // TODO: query string

        static URL parse(const std::string& strUrl) {
            // Sample URL: https://github.com/glfw/glfw.git

            URL url;

            const size_t schemaPos = strUrl.find_first_of(':');
            url.schema = strUrl.substr(0, schemaPos);

            const size_t hostEndPos = strUrl.find('/', schemaPos + 3);
            url.host = strUrl.substr(schemaPos + 3, hostEndPos - (schemaPos + 3));

            url.path = strUrl.substr(hostEndPos);

            return url;
        }
    };

    struct GitRepository {
        std::string url;

        void clone(CommandExecutor &execute, const boost::filesystem::path &sourcePath) const {
            if (boost::filesystem::exists(sourcePath)) {
                return;
            }

            CommandX command{
                "git", {"clone", url, sourcePath.string()}
            };

            execute(command);
        }
    };

    struct CMakeDefinition {
        std::string name;
        std::string value;
    };

    std::string evaluate(const CMakeDefinition& def) {
        return "-D" + def.name + "=\"" + def.value + "\"";
    }

    struct CMakeConfig {
        std::string sourcePath;
        std::string buildPath;
        std::string generator;
        std::map<std::string, std::string> definitions;
    };

    CommandX createVC2019VarsCommand() {
        return {
            "call",
            {"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\""}
        };
    }

    CommandX generateCommand(const CMakeConfig &config) {
        CommandX command {
            "cmake",
            {
                "-S" + config.sourcePath,
                "-B" + config.buildPath,
                "-G \"" + config.generator + "\""
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
        std::string url;
        std::map<std::string, std::string> definitions;
    };


    class DependencyManager {
    public:
        explicit DependencyManager(CommandExecutor &executor, const std::string& prefixPath) 
            : executor(executor), prefixPath(prefixPath) {}

        bool download(const Dependency& dependency) const {
            const auto repository = GitRepository { dependency.url };
            const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url));

            repository.clone(executor, sourcePath);

            return true;
        }

        bool configure(const Dependency& dependency, const CMakeBuildType buildType, const std::string &generator) {
            const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url));
            
            const auto buildPath = computePath(sourcePath, buildType);
            const auto installPath = computePath(prefixPath / "packages", URL::parse(dependency.url));

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
            const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url));
            const auto buildPath = computePath(sourcePath, buildType);
            
            CMakeBuild build { buildPath.string() };
            CommandX command = generateCommand(build);
            CommandBatch batch = createCMakeBatch(command);

            executor(batch);

            return true;
        }

        bool install(const Dependency& dependency, const CMakeBuildType buildType) {
            const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url));
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
                const CommandX vcvars = createVC2019VarsCommand();
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


        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const URL url) const {
            boost::filesystem::path sourcePath{ prefix / url.host };

            std::vector<std::string> pathParts;
            boost::split(pathParts, url.path, boost::is_any_of("/"));

            for (const auto& part : pathParts) {
                sourcePath /= part;
            }

            return sourcePath;
        }
            
    private:
        CommandExecutor &executor;
        boost::filesystem::path prefixPath;
    };
}


namespace Xenobuild {
    SetupControllerInput SetupControllerInput::parse(int, char**) {
        const auto currentPath = boost::filesystem::current_path();

        SetupControllerInput result;

        result.sourceDir = currentPath.string();
        result.buildDir = (currentPath / ".Xenobuild").string();

        return result;
    }
}


namespace Xenobuild {
    SetupController::SetupController(PackageFactory &packageFactory, const SetupControllerInput& params) 
        : packageFactory(packageFactory), params(params) {}

    void SetupController::perform() {
        const std::vector<Dependency> dependencies = {
            Dependency{
                "https://github.com/glfw/glfw.git", 
                {
                    { "GLFW_BUILD_DOCS", "OFF" },
                    { "GLFW_BUILD_EXAMPLES", "OFF" },
                    { "GLFW_BUILD_TESTS", "OFF" }
                }
            },
            Dependency {
                "https://github.com/google/fruit.git", 
                {
                    { "FRUIT_TESTS_USE_PRECOMPILED_HEADERS", "OFF" },
                    { "FRUIT_USES_BOOST", "OFF" }
                }
            },
            Dependency {
                "https://github.com/jbeder/yaml-cpp.git", 
                {
                    { "YAML_CPP_BUILD_TESTS", "OFF" }
                }
            }
        };

        // action: setup dependencies
        SystemCommandExecutor executor;
        DependencyManager manager{executor, "C:\\Users\\fapablaza\\.Xenobuild" };

        std::for_each(dependencies.begin(), dependencies.end(), [&manager](const Dependency& dep) {
            const CMakeBuildType buildTypes[] = {
                CMakeBuildType::Release, CMakeBuildType::Debug
            };

            manager.download(dep);

            for (const CMakeBuildType buildType : buildTypes) {
                manager.configure(dep, buildType, "NMake Makefiles");
                manager.build(dep, buildType);
                manager.install(dep, buildType);
            }
        });
    }
}
