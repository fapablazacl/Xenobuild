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
                                   const std::string &installSuffix);

        virtual ~DependencyManager() {}

        virtual bool download(const Dependency& dependency) const;

        virtual bool configure(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType, const boost::optional<CMakeGenerator> generator);

        virtual bool build(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType);

        virtual bool install(const Dependency& dependency, const Toolchain &toolchain, const CMakeBuildType buildType);
        
        virtual boost::filesystem::path computeInstallPath(const Dependency &dependency) const;

    private:
        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const CMakeBuildType type) const;

        boost::filesystem::path computePath(const boost::filesystem::path& prefix, const URL url, const std::string &suffix) const;

    private:
        CommandExecutor &executor;
        boost::filesystem::path prefixPath;
        std::string installSuffix;
    };    
}
