#pragma once 

#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>


namespace Xenobuild {
    struct Dependency;
    struct URL;
    
    enum class CMakeBuildType;
    enum class CMakeGenerator;
    
    class Toolchain;
    class CommandExecutor;
    
    class DependencyManager {
    public:
        explicit DependencyManager(CommandExecutor &executor,
                                   const std::string& prefixPath,
                                   const std::string &installSuffix,
                                   const unsigned processorCount);

        bool download(const Dependency& dependency) const;

        bool configure(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType, const boost::optional<CMakeGenerator> generator);

        bool build(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType);

        bool install(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType);
        
        boost::filesystem::path computeInstallPath(const Dependency &dependency, const CMakeBuildType buildType) const;

    private:
        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const CMakeBuildType type) const;

        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const URL url, const std::string &suffix) const;

    private:
        CommandExecutor &executor;
        boost::filesystem::path prefixPath;
        std::string installSuffix;
        unsigned processorCount = 1;
    };    
}
