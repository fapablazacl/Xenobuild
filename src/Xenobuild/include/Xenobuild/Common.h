
#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include <thread>

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>

#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Version.h>
#include <Xenobuild/core/Package.h>
#include <Xenobuild/core/PackageFactory.h>
#include <Xenobuild/core/Module.h>
#include <Xenobuild/core/SourceFile.h>


namespace Xenobuild {
    enum class Arch {
        X32,
        X64
    };
    
    inline std::ostream& operator<< (std::ostream &os, const Arch arch) {
        switch (arch) {
        case Arch::X32:
            os << "Arch::X32";
            break;
            
        case Arch::X64:
            os << "Arch::X64";
            break;
            
        default:
            os << "<Arch: Missing case>";
            break;
        }
        
        return os;
    }

    enum class OS {
        Windows,
        Linux,
        MacOS
    };
    
    
    inline std::ostream& operator<< (std::ostream &ostream, const OS os) {
        switch (os) {
        case OS::Windows:
            ostream << "OS::Windows";
            break;
            
        case OS::Linux:
            ostream << "OS::Linux";
            break;
            
        case OS::MacOS:
            ostream << "OS::MacOS";
            break;
            
        default:
            ostream << "<OS: Missing case>";
            break;
        }
        
        return ostream;
    }
    
    constexpr OS getHostOS() {
#if defined(_WIN32)
        return OS::Windows;
#elif defined(__APPLE__)
        return OS::MacOS;
#else
#error Current OS isn't supported.
#endif
    }
    
    enum class Toolchain {
        Clang,
        MicrosoftVC,
        AppleClang,
        GnuGCC
    };
    
    
    inline std::ostream& operator<< (std::ostream &ostream, const Toolchain toolchain) {
        switch (toolchain) {
        case Toolchain::Clang:
            ostream << "Toolchain::Clang";
            break;
            
        case Toolchain::MicrosoftVC:
            ostream << "Toolchain::MicrosoftVC";
            break;
            
        case Toolchain::AppleClang:
            ostream << "Toolchain::AppleClang";
            break;
            
        case Toolchain::GnuGCC:
            ostream << "Toolchain::GnuGCC";
            break;
            
        default:
            ostream << "<Toolchain: Missing case>";
            break;
        }
        
        return ostream;
    }
    
    
    struct Triplet {
        OS os;
        Arch arch;
        Toolchain toolchain;
        
        static Triplet nativeHost() {
            switch (getHostOS()) {
            case OS::Windows:
                return { OS::Windows, Arch::X64, Toolchain::MicrosoftVC };
                
            case OS::MacOS:
                return { OS::MacOS, Arch::X64, Toolchain::AppleClang };
                
            case OS::Linux:
                return { OS::Linux, Arch::X64, Toolchain::GnuGCC };
            }
        }
    };
    
    inline std::ostream& operator<< (std::ostream &os, const Triplet &triplet) {
        os << "Triplet { ";
        os << triplet.os << ", ";
        os << triplet.arch << ", ";
        os << triplet.toolchain << " }";
        
        return os;
    }

    
    inline unsigned getProcessorCount() {
        const auto processor_count = std::thread::hardware_concurrency();
        
        if (processor_count == 0) {
            return 1;
        }
        
        return processor_count;
    }

    inline std::string quote(const std::string& str) {
        return "\"" + str + "\"";
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
    
    struct CommandResult {
        int exitCode;
        std::vector<std::string> stdout;
        std::vector<std::string> stderr;
        
        operator bool() const {
            return exitCode == 0;
        }
    };
    
    inline std::ostream& write(std::ostream &os, std::vector<std::string> lines) {
        for (const std::string &line : lines) {
            os << line << std::endl;
        }
        
        return  os;
    }
    
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

        virtual CommandResult execute(const CommandX& command) = 0;

        virtual CommandResult execute(const CommandBatch& batch) = 0;

        inline CommandResult operator() (const CommandX& command) {
            return execute(command);
        }

        inline CommandResult operator() (const CommandBatch& batch) {
            return execute(batch);
        }
    };

    class SystemCommandExecutor : public CommandExecutor {
    public:
        virtual ~SystemCommandExecutor() {}

        CommandResult execute(const CommandX& command) override {
            const std::string cmdline = createCmdLine(command);
            const int code = std::system(cmdline.c_str());
            
            return {code, {}, {}};
        }

        CommandResult execute(const CommandBatch& batch) override {
            std::vector<std::string> cmdlines;

            std::transform(
                batch.commands.begin(),
                batch.commands.end(),
                std::back_inserter(cmdlines), [this](const auto command) {
                return createCmdLine(command);
            });

            const std::string cmdline = boost::join(cmdlines, "&&");

            const int code = std::system(cmdline.c_str());
            
            return {code, {}, {}};
        }

    private:
        std::string createCmdLine(const CommandX& command) const {
            return command.name + " " + boost::join(command.args, " ");
        }
    };

    
    class BoostProcessCommandExecutor : public CommandExecutor {
    public:
        virtual ~BoostProcessCommandExecutor() {}

        CommandResult execute(const CommandX& command) override {
            boost::filesystem::path commandPath = boost::process::search_path(command.name);
            
            if (! boost::filesystem::exists(commandPath)) {
                throw std::runtime_error("\"" + command.name + "\": Command couldn't be located within the current environment.");
            }
            
            boost::process::ipstream stdoutStream;
            boost::process::ipstream stderrStream;
            
            const std::string commandLine = commandPath.string() + " " + boost::join(command.args, " ");
            
            boost::process::child process {
                // commandPath, boost::process::args += command.args,
                commandLine,
                boost::process::std_out > stdoutStream,
                boost::process::std_err > stderrStream
            };

            const std::vector<std::string> stdoutOutput = grabStreamOutput(stdoutStream);
            const std::vector<std::string> stderrOutput = grabStreamOutput(stderrStream);

            process.wait();
            
            return {process.exit_code(), stdoutOutput, stderrOutput };
        }
        
        
        CommandResult execute(const CommandBatch& batch) override {
            assert(batch.commands.size() < 2);
            
            if (batch.commands.size() == 0) {
                return {0, {}, {}};
            }
            
            if (batch.commands.size() == 1) {
                return execute(batch.commands[0]);
            }
            
            // TODO: Add implementation
            return {0, {}, {}};
        }

    private:
        std::vector<std::string> grabStreamOutput(boost::process::ipstream &stream) const {
            std::string line;
            std::vector<std::string> lines;

            while (stream && std::getline(stream, line) && !line.empty()) {
                lines.push_back(line);
            }

            return lines;
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
    
    class DependencyManager {
    public:
        explicit DependencyManager(CommandExecutor &executor,
                                   const std::string& prefixPath,
                                   const std::string &toolchainPrefix,
                                   const std::string &installSuffix,
                                   const unsigned processorCount) :
        executor(executor),
        prefixPath(prefixPath),
        toolchainPrefix(toolchainPrefix),
        installSuffix(installSuffix) {}

        bool download(const Dependency& dependency) const {
            const URL url = URL::parse(dependency.url);
            const auto repository = GitRepository { dependency.url, dependency.tag };
            const auto sourcePath = computePath(prefixPath / "sources", url, dependency.tag);

            const CommandResult result = repository.clone(executor, sourcePath);

            if (!result) {
                std::cerr << "Repository failed." << std::endl;
                write(std::cerr, result.stderr);
                
                return false;
            }
            
            return true;
        }

        bool configure(const Dependency& dependency, const CMakeBuildType buildType, const std::string &generator) {
            const URL url = URL::parse(dependency.url);
            
            const auto sourcePath = computePath(prefixPath / "sources", url, dependency.tag);
            
            const auto buildPath = computePath(sourcePath, buildType);
            const auto installPath = computePath(prefixPath / "packages" / installSuffix, url, dependency.version);

            CMakeConfig config {
                sourcePath.string(),
                buildPath.string(),
                generator,
                createConfigDefinitions(installPath, buildType)
            };

            config.definitions.insert(dependency.definitions.begin(), dependency.definitions.end());

            CommandX command = generateCommand(config);
            CommandBatch batch = createCMakeBatch(command, toolchainPrefix);

            const CommandResult result = executor(batch);
            
            if (!result) {
                std::cerr << "Configure command failed." << std::endl;
                write(std::cerr, result.stderr);
                
                return false;
            }

            return true;
        }

        bool build(const Dependency& dependency, const CMakeBuildType buildType) {
            const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url), dependency.tag);
            const auto buildPath = computePath(sourcePath, buildType);
            
            // FIX: Using parallel build with CMake, causes deadlock in glfw.
            CMakeBuild build { buildPath.string()/*, processorCount */};
            CommandX command = generateCommand(build);
            CommandBatch batch = createCMakeBatch(command, toolchainPrefix);

            const CommandResult result = executor(batch);
            
            if (!result) {
                std::cerr << "Build command failed." << std::endl;
                write(std::cerr, result.stderr);
                
                return false;
            }

            return true;
        }

        bool install(const Dependency& dependency, const CMakeBuildType buildType) {
            const auto sourcePath = computePath(prefixPath / "sources", URL::parse(dependency.url), dependency.tag);
            const auto buildPath = computePath(sourcePath, buildType);
            
            CMakeInstall install { buildPath.string() };
            CommandX command = generateCommand(install);
            CommandBatch batch = createCMakeBatch(command, toolchainPrefix);

            const CommandResult result = executor(batch);
            
            if (!result) {
                std::cerr << "Install command failed." << std::endl;
                write(std::cerr, result.stderr);
                
                return false;
            }

            return true;
        }


    private:
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


        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const URL url, const std::string &suffix) const {
            boost::filesystem::path sourcePath{ prefix / url.host };

            std::vector<std::string> pathParts;
            boost::split(pathParts, url.path, boost::is_any_of("/"));

            for (const auto& part : pathParts) {
                sourcePath /= part;
            }

            sourcePath /= suffix;

            return sourcePath;
        }

    private:
        CommandExecutor &executor;
        boost::filesystem::path prefixPath;
        boost::filesystem::path toolchainPrefix;
        std::string installSuffix;
        unsigned processorCount;
    };
    
    
    inline std::string computePathSuffix(const OS os) {
        switch (os) {
        case OS::Windows:
            return "windows";

        case OS::Linux:
            return "linux";

        case OS::MacOS:
            return "macos";
        }

        return "host";
    }

    
    inline std::string computePathSuffix(const Arch arch) {
        switch (arch) {
        case Arch::X32:
            return "x86";

        case Arch::X64:
            return "x64";
        }

        return "native";
    }
    

    inline std::string computePathSuffix(const Toolchain toolchain) {
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

    inline std::string computePathSuffix(const Triplet& triplet) {
        return
            computePathSuffix(triplet.os) + "-" +
            computePathSuffix(triplet.arch) + "-" +
            computePathSuffix(triplet.toolchain);
    }
    
    /**
     * @brief Get the home folder of the current user.
     */
     inline boost::filesystem::path getUserPath() {
        switch (getHostOS()) {
            case OS::Windows:
                return std::getenv("USERPROFILE");
                
            case OS::MacOS:
            case OS::Linux:
                return std::getenv("HOME");
                
            default:
                return ".";
        }
    }
    
    
    class PackageManager {
    public:
        explicit PackageManager(CommandExecutor &executor,
                                   const std::string& prefixPath,
                                   const std::string &toolchainPrefix,
                                   const std::string &installSuffix,
                                   const unsigned processorCount) :
        executor(executor),
        prefixPath(prefixPath),
        toolchainPrefix(toolchainPrefix),
        installSuffix(installSuffix) {}
        
        bool configure(const std::vector<Dependency> &dependencies, const Package &package, const Triplet &triplet, const CMakeBuildType buildType) {
            // NOTE: Let's assume that the build system that the current Package uses, is CMake.
            CMakeConfig config;
            
            config.sourcePath = package.path.string();
            config.buildPath = (package.path / computePathSuffix(triplet) / evaluate(buildType)).string();
            config.definitions = createConfigDefinitions("", buildType);
            CommandX command = generateCommand(config);
            CommandBatch batch = createCMakeBatch(command, toolchainPrefix);

            const CommandResult result = executor(batch);
            
            if (!result) {
                std::cerr << "Configure command failed." << std::endl;
                write(std::cerr, result.stderr);
                
                return false;
            }
            
            return true;
        }
        
    private:
        CommandExecutor &executor;
        
        boost::filesystem::path prefixPath;
        
        //! Toolchain prefix used to configure this dependency.
        boost::filesystem::path toolchainPrefix;
        
        //! the install Suffix used during the dependencies installation.
        //! It's used to locate them during the configuration time.
        std::string installSuffix;
        
        //! currently detected processor count.
        //! used for paralelizing tasks
        unsigned processorCount = 1;
    };
}
