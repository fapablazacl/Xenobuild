
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


namespace Xenobuild {
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
    
    class CommandExecutor {
    public:
        virtual ~CommandExecutor() {}

        virtual void execute(const std::vector<std::string>& command) = 0;

        inline void operator() (const std::vector<std::string>& command) {
            execute(command);
        }
    };

    class SystemCommandExecutor : public CommandExecutor {
    public:
        virtual ~SystemCommandExecutor() {}

        void execute(const std::vector<std::string>& command) override {
            const std::string cmdline = boost::join(command, " ");
            const int code = std::system(cmdline.c_str());

            if (code != 0) {
                const std::string msg = "Error while executing command '" + cmdline + "'";
                throw CommandExecutorException(msg, code);
            }
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

        void clone(CommandExecutor &execute, const std::string &sourcePath) const {
            std::vector<std::string> command{
                "git", "clone", url, sourcePath
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


    std::vector<std::string> generateCommand(const CMakeConfig &config) {
        std::vector<std::string> command {
            "cmake",
            "-S" + config.sourcePath,
            "-B" + config.buildPath,
            "-G \"" + config.generator + "\""
        };

        std::transform(
            config.definitions.begin(), 
            config.definitions.end(), 
            std::back_inserter(command), [](const auto pair) {

            const CMakeDefinition definition{ pair.first, pair.second };

            return evaluate(definition);
        });

        return command;
    }


    struct CMakeProject {
        std::string sourcePath;

        void configure(
            CommandExecutor &execute, 
            const std::string &buildType, 
            const std::string &buildPath, 
            const std::string &generator,
            const std::optional<std::string> &installPath, 
            const std::optional<std::string> &debugPostfix,
            const std::vector<std::string> &prefixPaths,
            const std::map<std::string, std::string> &definitionMap) {

            std::vector<std::string> command {
                "cmake",
                "-S" + sourcePath,
                "-B" + buildPath,
                "-G \"" + generator + "\"",
                "-DCMAKE_OSX_ARCHITECTURES=\"arm64\"",
                "-DCMAKE_BUILD_TYPE=" + buildType
            };

            if (installPath.has_value()) {
                const std::string value = "-DCMAKE_INSTALL_PREFIX=\"" + installPath.value() + "\"";
                command.push_back(value);
            }

            if (debugPostfix.has_value()) {
                const std::string definition = "-DCMAKE_DEBUG_POSTFIX=\"" + debugPostfix.value() + "\"";

                command.push_back(definition);
            }

            if (prefixPaths.size() > 0) {
                const std::string value = "-DCMAKE_PREFIX_PATH=\"" + boost::join(prefixPaths, ";") + "\"";
                command.push_back(value);
            }
            
            for (const auto& def : definitionMap) {
                const std::string value = "-D" + def.first + "=" + def.second;
                command.push_back(value);
            }

            execute(command);
        }

        void build(CommandExecutor &execute, std::string buildPath) {
            std::vector<std::string> command = {
                "cmake",
                "-B" + buildPath,
                "--build"
            };

            execute(command);
        }

        void install(CommandExecutor &execute, std::string buildPath) {
            std::vector<std::string> command = {
                "cmake",
                "-B" + buildPath,
                "--install"
            };

            execute(command);
        }
    };

    struct Dependency {
        std::string url;
        std::map<std::string, std::string> definitions;
    };

    struct DependencyInstallResult {
        bool success = false;
        std::string message;

        operator bool() const {
            return success;
        }
    };

    class DependencyManager {
    public:
        explicit DependencyManager(CommandExecutor &executor, const std::string& prefixPath) 
            : executor(executor), prefixPath(prefixPath) {}

        DependencyInstallResult install(const Dependency& dependency) {
            const auto repository = GitRepository { dependency.url };
            const auto sourcePath = computeSourcePath(prefixPath / "sources", URL::parse(dependency.url));

            // assumtions:
            // the source URL is a Git repository
            // the build system in that directory is CMake

            return {true, ""};
        }

    private:
        boost::filesystem::path computeSourcePath(const boost::filesystem::path& sourcePrefixPath, const URL url) const {
            boost::filesystem::path sourcePath{ sourcePrefixPath / url.host };

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
        std::cout << "SetupController::perform" << std::endl;

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

        manager.install(dependencies[0]);

        // TODO: Locate user directory, and put the downloaded source code and residual 
        // build artifacts in one sub-location, and the installation path in another
        // 
        // Package package = packageFactory.createPackage(params.sourceDir);
        // print(package);
    }
}
