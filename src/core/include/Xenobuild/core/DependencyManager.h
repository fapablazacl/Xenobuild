#pragma once 

#include <string>
#include <boost/filesystem/path.hpp>


namespace Xenobuild {
    struct Dependency;
    struct URL;
    
    enum class CMakeBuildType;
    
    class CommandExecutor;
    
    class DependencyManager {
    public:
        explicit DependencyManager(CommandExecutor &executor,
                                   const std::string& prefixPath,
                                   const std::string &toolchainPrefix,
                                   const std::string &installSuffix,
                                   const unsigned processorCount);

        bool download(const Dependency& dependency) const;

        bool configure(const Dependency& dependency, const CMakeBuildType buildType, const std::string &generator);

        bool build(const Dependency& dependency, const CMakeBuildType buildType);

        bool install(const Dependency& dependency, const CMakeBuildType buildType);
        
        boost::filesystem::path computeInstallPath(const Dependency &dependency, const CMakeBuildType buildType) const;

    private:
        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const CMakeBuildType type) const;

        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const URL url, const std::string &suffix) const;

    private:
        CommandExecutor &executor;
        boost::filesystem::path prefixPath;
        boost::filesystem::path toolchainPrefix;
        std::string installSuffix;
        unsigned processorCount = 1;
    };    
}
