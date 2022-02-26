
#pragma once 

#include <vector>
#include <string>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>

#include "Module.h"
#include "Dependency.h"

namespace Xenobuild {
    struct Module;
    struct Dependency;

    /**
     * @brief Represents a Software Package.
     */
    struct Package {
        //! package name
        std::string name;

        //! a short description
        std::string description;

        //! Root directory path, where the source code is located. This is optional
        boost::filesystem::path path;

        //! the list of modules that compose the package
        std::vector<Module> modules;

        //! the list of Dependency libraries required for all modules in the Package
        std::vector<Dependency> dependencies;
    };
}
