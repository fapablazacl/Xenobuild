
#pragma once 

#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>


namespace Xenobuild {
    enum class CMakeBuildType;
    
    struct Dependency;
    struct Package;
    struct Triplet;

    class Toolchain;
    class CommandExecutor;
    class DependencyManager;
    
    class PackageManager {
    public:
        explicit PackageManager(CommandExecutor &executor,
                                const std::string& prefixPath,
                                const std::string &installSuffix,
                                const unsigned processorCount);
        
        bool configure(const Package &package, const Toolchain &toolchain, const Triplet &triplet, const CMakeBuildType buildType, const DependencyManager &dependencyManager);
            
    private:
        CommandExecutor &executor;
        
        boost::filesystem::path prefixPath;
        
        //! the install Suffix used during the dependencies installation.
        //! It's used to locate them during the configuration time.
        std::string installSuffix;
        
        //! currently detected processor count.
        //! used for paralelizing tasks
        unsigned processorCount = 1;
    };
}
