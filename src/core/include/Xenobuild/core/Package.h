
#pragma once 

#include <vector>
#include <string>
#include <boost/filesystem/path.hpp>

namespace Xenobuild {
    struct Module;
    struct Dependency;
    
    struct Package {
        std::string name;
        boost::filesystem::path path;
        std::vector<Module> modules;
        std::vector<Dependency> dependencies;
        
        ~Package();
    };
}
