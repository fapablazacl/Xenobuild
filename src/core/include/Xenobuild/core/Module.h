
#pragma once 

#include <vector>
#include <string>
#include <boost/filesystem/path.hpp>

#include "SourceFile.h"

namespace Xenobuild {
    enum class ModuleType {
        Executable,
        Library
    };

    struct Module {
        std::string name;
        ModuleType type = ModuleType::Executable;
        std::string lang;
        boost::filesystem::path path;
        std::vector<std::string> dependencies;
        std::vector<SourceFile> sourceFiles;
    };
}
