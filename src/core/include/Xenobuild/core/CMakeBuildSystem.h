
#pragma once 

#include <string>
#include <map>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>


namespace Xenobuild {
    struct CommandX;
    struct CommandBatch;

    struct CMakeDefinition {
        std::string name;
        std::string value;
    };
    
    enum class CMakeGenerator {
        UnixMakefiles,
        NMakeMakefiles,
        MinGWMakefiles
    };

    std::string evaluate(const CMakeGenerator generator);

    struct CMakeConfiguration {
        std::string sourcePath;
        std::string buildPath;
        boost::optional<CMakeGenerator> generator;
        std::map<std::string, std::string> definitions;
    };

    struct CMakeBuild {
        std::string buildPath;
    };

    struct CMakeInstall {
        std::string buildPath;
    };

    enum class CMakeBuildType {
        Default,
        Debug,
        Release
    };

    std::string evaluate(const CMakeDefinition& def);
    
    CommandX generateCommand(const CMakeConfiguration &config);
    
    CommandX generateCommand(const CMakeBuild& build);
    
    CommandX generateCommand(const CMakeInstall& install);
    
    std::string evaluate(const CMakeBuildType buildType);

    std::map<std::string, std::string> createConfigDefinitions(
        const boost::filesystem::path& installPrefix, 
        const CMakeBuildType buildType);

    struct CMakeProject {
        std::string sourcePath;
        std::vector<CMakeConfiguration> configurations;
    };
}
