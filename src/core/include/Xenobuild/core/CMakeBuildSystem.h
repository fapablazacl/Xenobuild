
#pragma once 

#include <string>
#include <map>
#include <boost/filesystem/path.hpp>


namespace Xenobuild {
    struct CommandX;
    struct CommandBatch;

    struct CMakeDefinition {
        std::string name;
        std::string value;
    };

    struct CMakeConfig {
        std::string sourcePath;
        std::string buildPath;
        std::string generator;
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
    
    CommandX generateCommand(const CMakeConfig &config);
    
    CommandX generateCommand(const CMakeBuild& build);
    
    CommandX generateCommand(const CMakeInstall& install);
    
    std::string evaluate(const CMakeBuildType buildType);

    std::map<std::string, std::string> createConfigDefinitions(const boost::filesystem::path& installPrefix, const CMakeBuildType buildType);
}
