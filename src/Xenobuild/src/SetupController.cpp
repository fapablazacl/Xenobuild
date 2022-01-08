
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
#include <boost/algorithm/string/join.hpp>


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

        virtual void execute(const std::vector<std::string>& command) throw(CommandExecutorException) = 0;

        inline void operator() (const std::vector<std::string>& command) throw(CommandExecutorException) {
            execute(command);
        }
    };

    class SystemCommandExecutor : public CommandExecutor {
    public:
        virtual ~SystemCommandExecutor() {}

        void execute(const std::vector<std::string>& command) throw(CommandExecutorException) override {
            const std::string cmdline = boost::join(command, " ");
            const int code = std::system(cmdline.c_str());

            if (code != 0) {
                const std::string msg = "Error while executing command '" + cmdline + "'";
                throw CommandExecutorException(msg, code);
            }
        }
    };

    struct GitRepository {
        std::string url;

        void clone(CommandExecutor &execute, const std::string &sourcePath) {
            std::vector<std::string> command{
                "git", "clone", url, sourcePath
            };

            execute(command);
        }
    };

    struct CMakeProject {
        std::string sourcePath;

        void configure(
            CommandExecutor &execute, 
            const std::string &buildType, 
            const std::string &buildPath, 
            const std::string &generator,
            const std::optional<std::string> &installPath, 
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

            if (prefixPaths.size() > 0) {
                const std::string value = "-DCMAKE_PREFIX_PATH=\"" + boost::join(prefixPaths, ";") + "\"";
                command.push_back(value);
            }
            
            for (const auto& def : definitionMap) {
                const std::string value = "- D" + def.first + "=" + def.second;
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

    struct CMakeOptions {
        std::map<std::string, std::string> definitionMap;
    };

    struct Dependency {
        std::string url;
        std::optional<CMakeOptions> cmakeOptions;
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

        // Package package = packageFactory.createPackage(params.sourceDir);
        // print(package);
    }
}
