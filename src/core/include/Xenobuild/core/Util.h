
#pragma once

#include <string>
#include <vector>
#include <map>
#include <iosfwd>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>


namespace Xenobuild {
    struct CommandBatch;
    struct CommandX;
    struct CMakeDefinition;
    struct CMakeBuild;
    struct CMakeInstall;
    struct CMakeConfiguration;
    
    enum class CMakeBuildType;
    
    unsigned getProcessorCount();

    std::string quote(const std::string& str);
    
    std::ostream& write(std::ostream &os, std::vector<std::string> lines);

    boost::filesystem::path getUserPath();

    boost::optional<std::string> getenv(const std::string& var);
}
