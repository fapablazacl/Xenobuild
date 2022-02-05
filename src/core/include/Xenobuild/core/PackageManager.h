
#pragma once 


namespace Xenobuild {
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
