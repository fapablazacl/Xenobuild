
#pragma once

#include <string>
#include <vector>
#include <map>
#include <iosfwd>
#include <boost/filesystem/path.hpp>


namespace Xenobuild {
    struct CommandBatch;
    struct CommandX;
    struct CMakeDefinition;
    struct CMakeBuild;
    struct CMakeInstall;
    struct CMakeConfig;
    
    enum class CMakeBuildType;
    
    unsigned getProcessorCount();

    std::string quote(const std::string& str);
    
    std::ostream& write(std::ostream &os, std::vector<std::string> lines);
    
    std::string evaluate(const CMakeDefinition& def);
    
    std::vector<std::string> enumerateVCInstallations();

    CommandX createVCVars64Command(const boost::filesystem::path &prefixPath);

    CommandX generateCommand(const CMakeConfig &config);
    
    CommandX generateCommand(const CMakeBuild& build);
    
    CommandX generateCommand(const CMakeInstall& install);
    
    std::string evaluate(const CMakeBuildType buildType);

    std::map<std::string, std::string> createConfigDefinitions(const boost::filesystem::path& installPrefix, const CMakeBuildType buildType);
    
    CommandBatch createCMakeBatch(const CommandX command, const boost::filesystem::path &toolchainPrefix);

    boost::filesystem::path getUserPath();
}
