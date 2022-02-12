
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
    
    std::vector<std::string> enumerateVCInstallations();

    CommandX createVCVars64Command(const boost::filesystem::path &prefixPath);

    CommandBatch createCMakeBatch(const CommandX command, const boost::filesystem::path &toolchainPrefix);

    boost::filesystem::path getUserPath();
}
