
#pragma once 

#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem/path.hpp>

#include "Module.h"

namespace Xenobuild {
    struct Package {
        std::string name;
        boost::filesystem::path path;
        std::vector<Module> modules;
        
    };

    inline void print(const Package &package) {
        std::cout << "Package " << package.name << std::endl;
        for (const Module &module : package.modules) {
            std::cout << "    Module " << module.name << std::endl;

            for (const SourceFile &source : module.sourceFiles) {
                std::cout << "        SourceFile " << source.path << std::endl;
            }
        }
    }
}
