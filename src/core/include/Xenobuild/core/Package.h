
#pragma once 

#include <vector>
#include <string>
#include <boost/filesystem/path.hpp>

#include "Module.h"

namespace Xenobuild {
    struct Package {
        std::string name;
        boost::filesystem::path path;
        std::vector<Module> modules;
    };
}
