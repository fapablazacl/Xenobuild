
#pragma once 

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>


namespace Xenobuild {
    enum class CMakeBuildType;
    
    struct Dependency;
    struct Package;
    struct Triplet;
    struct Toolchain;

    class CommandExecutor;
    class DependencyManager;
    
    class PackageManager {
    public:
        explicit PackageManager(CommandExecutor &executor,
                                   const std::string& prefixPath,
                                   const std::string &toolchainPrefix,
                                   const std::string &installSuffix,
                                const unsigned processorCount);
        
        bool configure(const Package &package, const Triplet &triplet, const CMakeBuildType buildType, const DependencyManager &dependencyManager);
            
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
